/*	
Project3 - Morgan Ludtke & Faiz Lurman

*/

#include <iostream>
#include <cassert>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "background.h"
#include "sphere.h"
#include "map.h"
#include "JumboTron.h"
#include "Skybox.h"
#include "Box2D\Box2D.h"
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
	GLint slices;	//number of slices
	GLint stacks;	//number of stacks
	GLint shader;	//current shader
	GLint num_balls;	//number of balls on field
	GLfloat ball_radius;	//radius of the balls on the field
	GLfloat obstacle_height, obstacle_width;	//dimensions of obstacles on field
	string title;
	ivec2 size;
	GLfloat aspect;
	vector<string> instructions;	//text on screen
	vector<string> plus_sign;
	vector<string> small_screen;
} window, window2;

//create objects
Sphere ball;
Map ground, walls, cursor;	//map objects
JumboTron tron;	//JumboTrons
vector<Sphere> balls; //balls
Skybox sky;
Sphere baller;
Obstacle box;
Scoreboard score;
FrameBufferObject fbo;

static GLfloat xrot = 0.0f, yrot = 0.0f; //used for mouse movement
static GLfloat xdiff = 0.0f, ydiff = 0.0f;	//used for mouse movement (old)
GLfloat radius, xpos, ypos, zpos;	//used for camera movement
GLfloat angleH, angleV;	//ditto
GLfloat cameraX, cameraZ;
GLfloat lookatX = 0.0f, lookatY = 0.0f, lookatZ = 0.0f; //first person camera movement

float PI = 3.1415926f;

bool gameWon = false;

GLfloat xDiffFromCenter, yDiffFromCenter; //used for mouse movement
GLint facing_direction = 1;

int debug_mode = 1;	//keeps track of what debug mode it is in

//box2D
b2BodyDef groundBodyDef;
b2PolygonShape groundBox;
b2BodyDef bodyDef;

void DisplayInstructions()
{
	if (window.handle == -1)
		return;

	vector<string> * s = &window.instructions;
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
	glTranslated(10, 15 * s->size(), -5.5);
	glScaled(0.1, 0.1, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}

void DisplayCursor()
{
	if (window.handle == -1)
		return;

	vector<string> * s = &window.plus_sign;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window.size.x, 0, window.size.y, 1, 10);
	glViewport(0, 0, window.size.x, window.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(window.size.x / 2.0 - 30, window.size.y / 2.0 - 30, -5.5);
	glScaled(0.6, 0.6, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}

void DisplayMAP()
{
	if (window.handle == -1)
		return;

	vector<string> * s = &window.plus_sign;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window.size.x, 0, window.size.y, 1, 10);
	glViewport(0, 0, window.size.x/4, window.size.y/4);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(window.size.x / 2.0 - s->size(), window.size.y / 2.0 - s->size(), -5.5);
	glScaled(0.6, 0.6, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}


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

void RenderIntoFrameBuffer()
{
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	fbo.Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(20, double(fbo.size.x) / double(fbo.size.y), 1, 10);
	glViewport(0, 0, fbo.size.x, fbo.size.y);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
	glRotatef(-time * 60.0f, 1.0f, 1.0f, 0.0f);
	Axes();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.5f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	fbo.Unbind();
}




void UseFramebufferToDrawSomething(mat4 m, mat4 p)
{
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(15, 10.0/5.0, 1, 10);
	//glViewport(0, 0, window.size.x/2.0, window.size.y/2.0);
	glMatrixMode(GL_MODELVIEW);
	ball.Draw(p, m, window.size);
	glLoadIdentity();
	gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
	//glRotatef(time * 30.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-30, 1.0f, 0.0f, 0.0f);
	Axes();
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}




void CloseFunc()	//Makes sure everything is deleted when the window is closed
{
	window.handle = -1;
	ball.TakeDown();
	ground.TakeDown();
	walls.TakeDown();
	tron.TakeDown();
	sky.TakeDown();
	box.TakeDown();
	score.TakeDown();
	for (int i = 0; i < balls.size(); i++)
	{
		balls.at(i).TakeDown();
	}

	cout << " Elapsed Time: " << GLfloat(glutGet(GLUT_ELAPSED_TIME)) / 1E3 << "s" << endl;
	if (gameWon)
		cout << "YOU WON MOSHBALL!  THROW THE CONFETTI" << endl;
	if (!gameWon)
		cout << "you lost, sorry" << endl;
}

void ReshapeFunc(int w, int h)
{
	if (h > 0)
	{
		window.size = ivec2(w, h);
		window.aspect = float(w) / float(h);
	}
}

void KeyboardFunc(unsigned char c, GLint x, GLint y)
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch (c)
	{
	case 'n':
		
		break;

	case 'w':	//Turn to wireframe mode
		window.wireframe = !window.wireframe;
		break;

	case 'p':
		if (window.paused)
		{
			// Will be leaving paused state
			window.total_time_paused += (current_time - window.time_last_pause_began);
			debug_mode = 2;
		}
		else
		{
			// Will be entering paused state
			window.time_last_pause_began = current_time;
			debug_mode = 1;
		}
		window.paused = !window.paused;
		break;

	case 's':	//change shader
		window.shader++;
	
		break;

	case ' ':	//jump
		ypos = 3.0f;
		break;
	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

void SpecialFunc(GLint key, GLint xPt, GLint yPt)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:	//increases the number of vertices for all objects (slices and stacks)
		++window.slices;
		++window.stacks;
		ball.TakeDown();
		ball.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, ball.getTime());
		break;

	case GLUT_KEY_PAGE_DOWN:	//decreases the number of vertices for all objects
		if (window.slices > 1)
		{
			--window.slices;
			window.slices = clamp(window.slices, 2, 50);
			--window.stacks;
			window.stacks = clamp(window.stacks, 2, 50);
			ball.TakeDown();
			ball.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, ball.getTime());
			
		}
		break;

	case GLUT_KEY_F1:	//changes between debug modes and final view
		debug_mode++;
		if(debug_mode > 3)
			debug_mode = 0;
		break;

	}
	
	return;

}


void Box()
{
	b2Vec2 gravity(0.0f, -10.0f);
	bool doSleep = true;
	b2World world(gravity); 

	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	groundBox.SetAsBox(50.0f, 10.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
}


void DisplayFunc2()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glDisable(GL_CULL_FACE);

	//glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x/4, window.size.y/4);

	mat4 projection = perspective(30.0f, window.aspect, 1.0f, 600.0f);

	radius = 20.0f;

	float tempX = xpos;
	float tempZ = zpos;
	float tempLookatX = lookatX;
	float tempLookatZ = lookatZ;

	mat4 modelview;
	modelview = lookAt(vec3(0.0f, 250.0f, 0.01f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);

	//sky.Draw(projection, modelview, window.size);

	//Draw map elements (ground and walls)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	ground.Draw(projection, modelview, window.size);
	//walls.Draw(projection, modelview, window.size);
	//modelview = translate(modelview, vec3(0.0f, 1.0f, 0.0f));

	//Draw jumbotrons and scoreboards
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	tron.Draw(projection, modelview, window.size);
	//framebuffer testing
	//glEnable(GL_DEPTH_TEST);
	/*RenderIntoFrameBuffer();
	UseFramebufferToDrawSomething(modelview, projection);
	glViewport(0, 0, window.size.x, window.size.y);*/
	//
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	tron.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	score.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	score.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, -90.0f, vec3(0.0f, 1.0f, 0.0f));

	//Draw player (and other objects)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -10.0f));
	box.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 10.0f));
	modelview = translate(modelview, vec3(0.0f, window.ball_radius, 0.0f));	//make sure balls are on ground
	modelview = translate(modelview, vec3(xpos, ypos, zpos));
	ball.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(-xpos, -ypos, -zpos));

	for (int i = 0; i < balls.size(); i++)	//places the desired number of balls on field
	{
		modelview = translate(modelview, balls.at(i).getPostion());	//places in random location
		
		//balls.at(i).Draw(projection, modelview, window.size);
		ball.Draw(projection, modelview, window.size);
		//cout << balls.at(i).getTime() << endl;
		modelview = translate(modelview, -balls.at(i).getPostion());
	}


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glFlush();
	glutPostRedisplay(); // FPS
}

void DisplayFunc()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	

	glDisable(GL_CULL_FACE);

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);

	mat4 projection = perspective(20.0f, window.aspect, 1.0f, 600.0f);

	radius = 20.0f;

	float tempX = xpos;
	float tempZ = zpos;
	float tempLookatX = lookatX;
	float tempLookatZ = lookatZ;

	//movement (with a buffer zone in center) in first person view
	if (debug_mode != 1 && (xDiffFromCenter > 0.5f || xDiffFromCenter < -0.5f || yDiffFromCenter > 0.5f || yDiffFromCenter < -0.5f))
	{
		//move forward and backwards
		xpos = xpos + ((lookatX - xpos) * yDiffFromCenter * 0.001f);
		zpos = zpos + ((lookatZ - zpos) * yDiffFromCenter * 0.001f);
		ypos = ypos + ((0 - ypos) * 0.003f);
		lookatX = xpos - sin(angleV * (PI / 180)) * radius;
		lookatZ = zpos - cos(angleV * (PI / 180)) * radius;
		lookatY = ypos;
		
		zpos = clamp(zpos, -104.0f, 104.0f);
		xpos = clamp(xpos, -104.0f, 104.0f);

		// turn right or left
		angleV = angleV - xDiffFromCenter * 0.005f;
		lookatY = lookatY;
		lookatX = xpos - sin(angleV * (PI / 180)) * radius;
		lookatZ = zpos - cos(angleV * (PI / 180)) * radius;
	}

	//movement in third person view
	if (debug_mode == 1 && (xDiffFromCenter > 0.5f || xDiffFromCenter < -0.5f))
	{
		angleH = angleH - xDiffFromCenter * 0.005f;
		cameraX = sin(angleH * (PI / 180)) * 110.0f;
		cameraZ = cos(angleH * (PI / 180)) * 110.0f;

	}

	//check to see if going to hit a ball
	//for (int i = 0; i < balls.size(); i++)
	//{
	//	if (xpos >= (balls.at(i).getPostion().x - window.ball_radius) && (xpos)  <= (balls.at(i).getPostion().x + window.ball_radius))
	//	{
	//		if (zpos >= (balls.at(i).getPostion().z - window.ball_radius) && (zpos)  <= (balls.at(i).getPostion().z + window.ball_radius))
	//			
	//			xpos = tempX;
	//			zpos = tempZ;
	//			//lookatX = tempLookatX;
	//			//lookatZ = tempLookatZ;
	//			cout << balls.at(i).getPostion().x << ", " << balls.at(i).getPostion().z << ", " << i << endl;
	//	}
	//}

	mat4 modelview;
	if (debug_mode == 1)
	{
		modelview = lookAt(vec3(cameraX, 30.0f, cameraZ), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		modelview = lookAt(vec3(xpos, ypos, zpos), vec3(lookatX, lookatY, lookatZ), vec3(0.0f, 1.0f, 0.0f));
		gameWon = false;
	}
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);

	sky.Draw(projection, modelview, window.size);

	//Draw map elements (ground and walls)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	ground.Draw(projection, modelview, window.size);
	walls.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 1.0f, 0.0f));

	//Draw jumbotrons and scoreboards
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	tron.Draw(projection, modelview, window.size);
	//framebuffer testing
	//glEnable(GL_DEPTH_TEST);
	/*RenderIntoFrameBuffer();
	UseFramebufferToDrawSomething(modelview, projection);
	glViewport(0, 0, window.size.x, window.size.y);*/
	
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	tron.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	score.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	score.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, -90.0f, vec3(0.0f, 1.0f, 0.0f));

	//Draw player (and other objects)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -10.0f));
	box.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 10.0f));
	modelview = translate(modelview, vec3(0.0f, window.ball_radius, 0.0f));	//make sure balls are on ground
	modelview = translate(modelview, vec3(xpos, ypos, zpos));
	ball.Draw(projection, modelview, window.size);
	modelview = translate(modelview, vec3(-xpos, -ypos, -zpos));

	for (int i = 0; i < balls.size(); i++)	//places the desired number of balls on field
	{
		modelview = translate(modelview, balls.at(i).getPostion());	//places in random location
		
		//balls.at(i).Draw(projection, modelview, window.size);
		ball.Draw(projection, modelview, window.size);
		//cout << balls.at(i).getTime() << endl;
		modelview = translate(modelview, -balls.at(i).getPostion());
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DisplayInstructions();

	DisplayCursor();

	DisplayFunc2();

	glFlush();
	glutPostRedisplay(); // FPS
}


GLvoid TimerFunc(GLint value)	//keeps track of time to be displayed on window
{
	if (0 != value) // first time timer runs
	{
		ostringstream title, FPS, dimension, elapsedTime;
		title     << window.title << " : ";
		FPS       << (window.FPS *= 4) << " Frames Per Second ";
		dimension << "@ " << window.size.x << " x " << window.size.y;
		window.FPS = 0;
		elapsedTime.precision(0);
		elapsedTime << fixed;
		elapsedTime << " Elapsed Time: " << GLfloat(glutGet(GLUT_ELAPSED_TIME)) / 1E3 << "s";
		string windowTitle = title.str() + FPS.str() + dimension.str() + elapsedTime.str();
		glutSetWindowTitle(windowTitle.c_str());
	}
	glutTimerFunc(250, TimerFunc, 1); // 250ms = 1/4 of a second
}

GLvoid MouseFunc(GLint button, GLint state, GLint xPt, GLint yPt)
{
	xdiff = xPt - yrot;
	ydiff = -yPt + xrot;
}

GLvoid MotionFunc(GLint xPt, GLint yPt)
{
	yrot = xPt - xdiff;
	xrot = yPt + ydiff;
}

GLvoid passiveMotionFunc(GLint x, GLint y)
{
	GLfloat winX, winY;		//coordinates of the center of screen
	winX = window.size.x / 2.0f;
	winY = window.size.y / 2.0f;

	//the cursor's position related to the center of screen
	xDiffFromCenter = (x - winX) / 100;
	yDiffFromCenter = (winY - y) / 100;

	if (xDiffFromCenter < -1.0f || xDiffFromCenter > 1.0f)
		glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
	if (xDiffFromCenter < -1.0f && yDiffFromCenter > 1.0f)
		glutSetCursor(GLUT_CURSOR_TOP_LEFT_CORNER);
	if (xDiffFromCenter > 1.0f && yDiffFromCenter > 1.0f)
		glutSetCursor(GLUT_CURSOR_TOP_RIGHT_CORNER);
	if (xDiffFromCenter > 1.0f && yDiffFromCenter < -1.0f)
		glutSetCursor(GLUT_CURSOR_BOTTOM_RIGHT_CORNER);
	if (xDiffFromCenter < 1.0f && yDiffFromCenter < -1.0f)
		glutSetCursor(GLUT_CURSOR_BOTTOM_LEFT_CORNER);
	if (xDiffFromCenter > -1.0f && xDiffFromCenter < 1.0f && yDiffFromCenter > -1.0f && yDiffFromCenter < 1.0f)
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	if (xDiffFromCenter > -1.0f && xDiffFromCenter < 1.0f && (yDiffFromCenter < -1.0f || yDiffFromCenter > 1.0f))
		glutSetCursor(GLUT_CURSOR_UP_DOWN);
		
	
	//cout << "look at z: " << lookatZ << endl;
	//cout << "look at x:     " << lookatX << endl;
	/*cout << "y diff: " << yDiffFromCenter << endl;
	cout << "x diff:     " << xDiffFromCenter << endl;
	cout << "angle: " << angleV << endl;*/

}

GLint main(GLint argc, GLchar * argv[])
{
	radius = 20.0f;
	xpos = 0.0f;
	ypos = 0.0f;
	zpos = 20.0f;
	cameraX = 0.0f;
	cameraZ = 110.0f;
	angleH = 0.0f;
	angleV = 0.0f;
	window.num_balls = 10;
	window.ball_radius = 1.0f;
	window.obstacle_height = 2.0f;
	window.obstacle_width = 3.0f;

	glutInit(&argc, argv);
	glutInitWindowSize(1300, 960);
	glutInitWindowPosition(500, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	//box2D functions
	

	window.handle = glutCreateWindow(window.title.c_str());
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc); // our takedown actions
	glutDisplayFunc(DisplayFunc);


	glutPassiveMotionFunc(passiveMotionFunc);	//detect mouse movement
	glutSetCursor(GLUT_CURSOR_UP_DOWN);

	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); // ensure complete takedown of resources

	window.instructions.push_back("Time played: ");
	window.instructions.push_back("Targets remaining: ");

	window.plus_sign.push_back("+");

	window.small_screen.push_back("hello world");
	

	assert(GLEW_OK == glewInit());	

	//initialize all objects, and returns error if failed
	if (!ball.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, GLUT_ELAPSED_TIME))
		return 0;
	//cout << "ball: " << ball.shader.program_id << endl;
	if (!ground.InitializeFloor())
		return 0;
	if (!walls.InitializeWalls())
		return 0;
	if (!tron.InitializeCylinder())
		return 0;
	if (!sky.Initialize())
		return 0;
	if (!box.Initialize(window.obstacle_height, window.obstacle_width, window.shader))
		return 0;
	if (!score.Initialize())
		return 0;

	if (!fbo.Initialize(glm::ivec2(512, 512), 1, true))
	{
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}


	srand (5);		//seed generator
	for (int i = 0; i < window.num_balls; i++)
	{
		//Sphere baller;
		//baller.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, GLUT_ELAPSED_TIME, vec3(0.0f, 0.0f, 0.0f));
		balls.push_back(baller);

		int rand_int = rand() % 200 - 100;
		float rand_float = ((float) rand()) / (float) RAND_MAX;
		float rand_numberX = rand_float + float(rand_int);
		rand_int = rand() % 200 - 100;
		rand_float = ((float) rand()) / (float) RAND_MAX;
		float rand_numberZ = rand_float + float(rand_int);
		
		balls.at(i).setPosition(vec3(rand_numberX, 0.0f, rand_numberZ));
		cout << balls.at(i).getPostion().x << " hey " << endl;
	}
	for (int i = 0; i < balls.size(); i++)
	{
		//if (!balls.at(i).Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, GLUT_ELAPSED_TIME));
		//	return 0;
		//cout << balls.at(i).getPosition() << endl;
		//cout << " " << balls.at(i).shader.program_id << endl;
	}
	balls.at(5).setPosition(vec3(0.0f, 0.0f, 0.0f));
	cout << balls.at(5).getPostion().x << " hey " << endl;
	//cout << balls.size() << endl;

	assert(TextureManager::Inst()->LoadTexture((const char *) "stone.jpg", 0));

	glutMainLoop();
}
