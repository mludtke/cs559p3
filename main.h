#include <iostream>
#include <cassert>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sphere.h"
#include "map.h"
#include "JumboTron.h"
#include "Skybox.h"
#include "Obstacle.h"
#include "Scoreboard.h"

#include "fbo.h"

using namespace std;
using namespace glm;

class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->slices = 20;
		this->interval = 1000 / 120;
		this->handle = -1;
		this->stacks = 20;
		this->shader = 0;
		this->title  = "MoshBall";
	}

	GLfloat time_last_pause_began;
	GLfloat total_time_paused;
	GLboolean paused , wireframe, normals;
	GLint handle;
	GLint interval;
	GLint FPS;
	GLboolean gameWon; //If game was won
	GLboolean gameOver; //if game was lost
	GLboolean gameStart; //if game has started
	GLboolean betweenLevels; //if between levels
	GLboolean minimap; //if showing minimap
	GLboolean hit_something; //if had hit something
	GLboolean clock_on; //if clock is on
	GLint slices;	//number of slices
	GLint stacks;	//number of stacks
	GLint shader;	//current shader
	GLint num_balls;	//number of balls on field
	GLfloat ball_radius;	//radius of the balls on the field
	GLfloat obstacle_height, obstacle_width;	//dimensions of obstacles on field
	GLint num_boxes;	//number of obstacles on field
	GLint num_hit;	//current number of hit items
	GLint num_remaining;
	GLint stage;	//level of playing
	string title;
	ivec2 size, center;
	GLfloat aspect;
	vector<string> instructions;	//text on screen
	vector<string> plus_sign;
} window, window2;

//create objects
Sphere ball;	//normal ball
Sphere ball2;	//hit ball
Sphere player;  //The players ball
Sphere bomb;
Map ground, walls, miniWalls, cursor, ground2, clock, trueGround;	//map objects
JumboTron tron, screen;	//JumboTrons
vector<Sphere> balls; //balls
vector<Obstacle> boxes;
Skybox sky;
Sphere baller;
Obstacle crate;
Obstacle box;
Scoreboard score;
Scoreboard scoreScreen;
FrameBufferObject fbo; // jumbotron framebuffer
FrameBufferObject fbo2; //floor framebuffer
FrameBufferObject clockFbo;

string displayNumRemaining;
string displayNumBalls;
string displayLevel;

static GLfloat xrot = 0.0f, yrot = 0.0f; //used for mouse movement
static GLfloat xdiff = 0.0f, ydiff = 0.0f;	//used for mouse movement (old)
GLfloat radius, xpos, ypos, zpos;	//used for camera movement
GLfloat angleH, angleV, angleh;	//ditto
GLfloat cameraX, cameraY, cameraZ;
GLfloat lookatX = 0.0f, lookatY = 0.0f, lookatZ = 0.0f; //first person camera movement

float PI = 3.1415926f;

GLfloat xDiffFromCenter, yDiffFromCenter; //used for mouse movement
GLint facing_direction = 1;

int debug_mode = 1;	//keeps track of what debug mode it is in

//box2D
//b2BodyDef groundBodyDef;
//b2PolygonShape groundBox;
//b2BodyDef bodyDef;

void DisplayFunc();
void DisplayFunc2();
void DisplayFunc3();
void DisplayFuncFloor();

GLvoid setLevel()
{
	if (window.stage == 8)
	{
		window.gameWon = true;
		debug_mode = 2;
	}
	window.num_balls = window.stage * 2 + 5;
	window.num_boxes = window.stage + 2;
	//window.ball_radius = window.ball_radius - 0.1f;
	xpos = 0.0f;
	ypos = 0.0f;
	zpos = 20.0f;

	angleV = 0.0f;
	ball.TakeDown();
	ball.Initialize(0, window.slices, window.stacks);
	ball2.TakeDown();
	ball2.Initialize(1, window.slices, window.stacks);
	player.TakeDown();
	player.Initialize(2, window.slices, window.stacks);	//hit = 2 to show not a playing object

	srand (0);		//seed generator
	for (int i = 0; i < window.num_balls; i++)
	{
		balls.push_back(baller);

		int rand_int = rand() % 200 - 100;
		float rand_float = ((float) rand()) / (float) RAND_MAX;
		float rand_numberX = rand_float + float(rand_int);
		rand_int = rand() % 200 - 100;
		rand_float = ((float) rand()) / (float) RAND_MAX;
		float rand_numberZ = rand_float + float(rand_int);
		
		balls.at(i).setPosition(vec3(rand_numberX, 0.0f, rand_numberZ));

		balls.at(i).reset();
	}

	boxes.clear();
	for (int i = 0; i < window.num_boxes; i++)
	{
		boxes.push_back(crate);
		int rand_int = rand() % 200 - 100;
		float rand_numberX = float(rand_int);
		rand_int = rand() % 200 - 100;
		float rand_numberZ = float(rand_int);

		boxes.at(i).setPosition(vec3(rand_numberX, 0.0f, rand_numberZ));
		boxes.at(i).setForce(0.0f);
		boxes.at(i).setDirection(vec3(0.0f, 0.0f, 0.0f));
	}

	ostringstream convert4;
	convert4.clear();
	convert4 << window.stage;
	displayLevel = convert4.str();
	window.instructions.pop_back();
	window.instructions.pop_back();
	window.instructions.pop_back();
	window.instructions.push_back("Level: " + displayLevel);
	window.instructions.push_back("Targets remaining: " + displayNumRemaining + " / " + displayNumBalls);
	window.instructions.push_back("Time played: " );
}

//display center cursor
void DisplayCursor()
{
	if (window.handle == -1)
		return;

	vector<string> * s = &window.plus_sign;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window.size.x, 0, window.size.y, 1, 10);
	glViewport(0, 0, window.size.x, window.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(window.size.x / 2.0 - 30, window.size.y / 2.0 - 30, -5.5);
	glScaled(0.6, 0.6, 1.0);
	glLineWidth(3.0f);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
	glLineWidth(1.0f);
}

//used for testing purposes
void Axes()
{
	GLUquadric * q = gluNewQuadric();

	glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);

	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(q, 0.02, 0.02, 1.0, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(q, 0.02, 10, 10);
	glPopMatrix();

	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(q, 0.02, 0.02, 1.0, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(q, 0.02, 10, 10);
	glPopMatrix();

	glColor3f(0.0f, 0.0f, 1.0f);
	gluCylinder(q, 0.02, 0.02, 1.0, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluSphere(q, 0.02, 10, 10);
	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(q, 0.011, 10, 10);
	gluDeleteQuadric(q);
}

//Render a screen into a framebuffer
void RenderIntoFrameBuffer(mat4 m, mat4 p)
{

	fbo.Bind();
	DisplayFunc3();
	fbo.Unbind();
	
}

//render the scoreboard into a framebuffer
void RenderIntoFrameBuffer2(mat4 m, mat4 p)
{
	fbo2.Bind();
	DisplayFuncFloor();
	fbo2.Unbind();
}
