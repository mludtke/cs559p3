/*	
Project3 - Morgan Ludtke & Faiz Lurman

*/

#include "main.h"

void DisplayTime(mat4 modelview, mat4 projection, float time, vec3 position)
{
	ostringstream convert;
	int integerTime = (int)time;
	convert << integerTime;
	string displayNumber;
	displayNumber = convert.str();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(projection));
	//glLoadIdentity();
	//glOrtho(0, window.size.x / 2.0, 0, window.size.y / 2.0, 1, 10);
	//glOrtho();
	glViewport(0, 0, window.size.x, window.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(modelview));
	//glLoadIdentity();
	glTranslatef(-0.2f + -0.175f * (3-1), window.ball_radius * 1.5f, 0.0f);
	glRotatef(angleV, 0.0f, 1.0f, 0.0f);
	glScaled(0.01, 0.01, 0.01);
	glPushMatrix();
	glutStrokeString(GLUT_STROKE_ROMAN, (const unsigned char*) displayNumber.c_str());
	glPopMatrix();
}

//Display words on screen
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
	glTranslated(window.size.x - 300, 15 * s->size(), -5.5);
	glScaled(0.1, 0.1, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}


//used for testing purposes
void UseFramebufferToDrawSomething(mat4 m, mat4 p)
{
	float time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	//glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(15, 10.0/5.0, 1, 10);*/
	mat4 projection = perspective(20.0f, window.aspect, 1.0f, 600.0f);
	//glViewport(0, 0, window.size.x/2.0, window.size.y/2.0);
	//glMatrixMode(GL_MODELVIEW);
	mat4 modelview;
	//ball.Draw(p, m, window.size);
	/*glLoadIdentity();
	gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);*/
	modelview = lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
	//glRotatef(time * 30.0f, 0.0f, 1.0f, 0.0f);
	//glRotatef(-30, 1.0f, 0.0f, 0.0f);
	/*Axes();
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
	glDisable(GL_TEXTURE_2D);*/

	////glViewport(0, 0, window.size.x/2.0, window.size.y/2.0);
	ball.Draw(projection, modelview, window.size, window.stage, window.shader);
	//glLoadIdentity();
	////glRotatef(time * 30.0f, 0.0f, 1.0f, 0.0f);
	//glRotatef(-30, 1.0f, 0.0f, 0.0f);
	//Axes();
	//glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex2f(-0.5f, -0.5f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex2f(0.5f, -0.5f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex2f(0.5f, 0.5f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex2f(-0.5f, 0.5f);
	//glEnd();
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);
}

void CloseFunc()	//Makes sure everything is deleted when the window is closed
{
	window.handle = -1;
	ball.TakeDown();
	ball2.TakeDown();
	ground.TakeDown();
	ground2.TakeDown();
	walls.TakeDown();
	tron.TakeDown();
	screen.TakeDown();
	sky.TakeDown();
	box.TakeDown();
	score.TakeDown();
	scoreScreen.TakeDown();
	player.TakeDown();
	bomb.TakeDown();

	cout << " Elapsed Time: " << GLfloat(glutGet(GLUT_ELAPSED_TIME)) / 1E3 << "s" << endl;
	if (window.gameWon)
		cout << "YOU WON MOSHBALL!  THROW THE CONFETTI" << endl;
	else
		cout << "you lost, sorry" << endl;
}

void ReshapeFunc(int w, int h)
{
	if (h > 0)
	{
		window.size = ivec2(w, h);
		window.center = ivec2(w/2, h/2);
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

	case 'W':
	case 'w':	//Turn to wireframe mode
		window.wireframe = !window.wireframe;
		break;

	case 'P':
	case 'p':
		if (window.paused)
		{
			// Will be leaving paused state
			window.total_time_paused += (current_time - window.time_last_pause_began);
		}
		else
		{
			// Will be entering paused state
			window.time_last_pause_began = current_time;
		}
		window.paused = !window.paused;
		break;

	case 's': case 'S':	
		//change shader
		window.shader++;
		//ball.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, 0.00f);
		//ball2.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, 0.00f);
		ball.StepShader();
		ball2.StepShader();
		break;

	case ' ':	//jump
		ypos = 3.0f;
		break;

	case 'L':
	case 'l': //shortcut to change levels
		window.stage++;
		if(window.stage > 7)
			window.stage = 1;
		setLevel();
		break;

	case 'M':
	case 'm':
		window.minimap = !window.minimap;
		break;

	case 13: //enter button (for starting play)
		if (window.gameOver || window.gameWon)
		{
			window.gameOver = false;
			window.gameWon = false;
			window.gameStart = false;
			window.stage = 1;
			setLevel();
			break;
		}
		window.gameStart = true;
		window.betweenLevels = false;
		if (debug_mode != 1)
			debug_mode = 1;
		break;

	case 'X':
	case 'x': //exit game
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
		ball2.TakeDown();
		ball2.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 1, ball.getTime());
		player.TakeDown();
		player.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 2, ball.getTime());
		bomb.TakeDown();
		bomb.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 3, ball.getTime());
		break;

	case GLUT_KEY_PAGE_DOWN:	//decreases the number of vertices for all objects
		if (window.slices > 1)
		{
			--window.slices;
			window.slices = clamp(window.slices, 2, 50);
			--window.stacks;
			window.stacks = clamp(window.stacks, 2, 50);
			ball2.TakeDown();
			ball2.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 1, ball.getTime());
			ball.TakeDown();
			ball.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, ball.getTime());
			player.TakeDown();
			player.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 2, ball.getTime());
			bomb.TakeDown();
			bomb.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 3, ball.getTime());
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


//void Box()
//{
//	b2Vec2 gravity(0.0f, -10.0f);
//	bool doSleep = true;
//	b2World world(gravity); 
//
//	groundBodyDef.position.Set(0.0f, -10.0f);
//	b2Body* groundBody = world.CreateBody(&groundBodyDef);
//	groundBox.SetAsBox(50.0f, 10.0f);
//
//	groundBody->CreateFixture(&groundBox, 0.0f);
//	bodyDef.type = b2_dynamicBody;
//	bodyDef.position.Set(0.0f, 4.0f);
//	b2Body* body = world.CreateBody(&bodyDef);
//}

//Jumbotron display function
void DisplayFunc3()
{
	float current_time = (glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	current_time = current_time - window.total_time_paused;

	glDisable(GL_CULL_FACE);

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, fbo.size.x, fbo.size.y);

	mat4 projection = perspective(20.0f, window.aspect, 1.0f, 600.0f);

	radius = 20.0f;

	mat4 modelview;

	modelview = lookAt(vec3(xpos, ypos, zpos), vec3(lookatX, ypos, lookatZ), vec3(0.0f, 1.0f, 0.0f));

	//Draw skybox
	sky.Draw(projection, modelview, window.size, window.stage, window.shader);

	//Draw map elements (ground and walls)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	ground.Draw(projection, modelview, window.size, window.stage, window.shader, current_time);
	walls.Draw_walls(projection, modelview, window.size);
	modelview = translate(modelview, vec3(0.0f, 1.0f, 0.0f));

	//Draw jumbotrons and scoreboards
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	tron.Draw(projection, modelview, window.size, window.stage, window.shader);
	screen.DrawScreen(projection, modelview, window.size, fbo, 0);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	tron.Draw(projection, modelview, window.size, window.stage, window.shader);
	screen.DrawScreen(projection, modelview, window.size, fbo, 0);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	score.Draw(projection, modelview, window.size, window.stage, window.shader);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
	score.Draw(projection, modelview, window.size, window.stage, window.shader);
	modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
	modelview = rotate(modelview, -90.0f, vec3(0.0f, 1.0f, 0.0f));

	//Draw player (and other objects)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, window.ball_radius, 0.0f));	//make sure balls are on ground
	modelview = translate(modelview, vec3(xpos, ypos, zpos));
	player.Draw(projection, modelview, window.size, window.stage, window.shader);
	modelview = translate(modelview, vec3(-xpos, -ypos, -zpos));

	window.num_hit = 0;
	for (int i = 0; i < (int)balls.size(); i++)	//places the desired number of balls on field
	{
		modelview = translate(modelview, balls.at(i).getPostion());	//places in random location
		if (balls.at(i).is_sphere_hit())
		{
			ball2.Draw(projection, modelview, window.size, window.stage, window.shader);
		}
		else
		{
			ball.Draw(projection, modelview, window.size, window.stage, window.shader);
		}
		modelview = translate(modelview, -balls.at(i).getPostion());
	}


	modelview = translate(modelview, vec3(0.0f, -window.ball_radius, 0.0f));
	for (int i = 0; i < (int)boxes.size(); i++)	//places the desired number of obstacles on field
	{
		modelview = translate(modelview, boxes.at(i).getPostion());	//places in random location
		box.Draw(projection, modelview, window.size, window.stage, window.shader);
		modelview = translate(modelview, -boxes.at(i).getPostion());
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	//glFlush();
	//glutSwapBuffers();
	//glutPostRedisplay(); // FPS
}

//Minimap display function
void DisplayFunc2()
{
	float current_time = (glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	current_time = current_time - window.total_time_paused;

	glDisable(GL_CULL_FACE);

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x/4, window.size.x/4);

	mat4 projection = perspective(35.0f, 1.0f, 1.0f, 600.0f);

	radius = 20.0f;

	float tempX = xpos;
	float tempZ = zpos;
	float tempLookatX = lookatX;
	float tempLookatZ = lookatZ;

	mat4 modelview;
	modelview = rotate(modelview, -90.0f, vec3(1.0f, 0.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, 350.0f, 0.0f));

	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);

	//Draw map elements (ground and walls)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	ground2.Draw(projection, modelview, ivec2(window.size.x/8, window.size.x/8), window.stage, window.shader, current_time);
	walls.Draw_walls(projection, modelview, window.size);
	//modelview = translate(modelview, vec3(0.0f, 1.0f, 0.0f));


	//Draw player (and other objects)
	modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
	modelview = translate(modelview, vec3(0.0f, window.ball_radius, 0.0f));	//make sure balls are on ground
	//modelview = translate(modelview, vec3(xpos * 0.75f, ypos - 60.0f, zpos * 0.75f));
	modelview = translate(modelview, vec3(xpos, ypos, zpos));
	player.Draw(projection, modelview, window.size, window.stage, window.shader);
	//modelview = translate(modelview, vec3(-xpos * 0.75f, -ypos + 60.0f, -zpos * 0.75f));
	modelview = translate(modelview, vec3(-xpos, -ypos, -zpos));


	for (int i = 0; i < (int)balls.size(); i++)	//places the desired number of balls on field
	{
		modelview = translate(modelview, balls.at(i).getPostion());	//places in random location
		//modelview = translate(modelview, vec3(balls.at(i).getPostion().x, -60.0f, balls.at(i).getPostion().z));
		if (i == 10)
			bomb.Draw(projection, modelview, window.size, window.stage, window.shader);
		else if (balls.at(i).is_sphere_hit())
		{
			ball2.Draw(projection, modelview, window.size, window.stage, window.shader);
		}
		else
		{
			ball.Draw(projection, modelview, window.size, window.stage, window.shader);
		}
		modelview = translate(modelview, -balls.at(i).getPostion() );
		//modelview = translate(modelview, vec3(-balls.at(i).getPostion().x * 0.75f, 60.0f, -balls.at(i).getPostion().z * 0.75));
	}

	for (int i = 0; i < (int)boxes.size(); i++)	//places the desired number of obstacles on field
	{
		modelview = translate(modelview, boxes.at(i).getPostion());	//places in random location
		box.Draw(projection, modelview, window.size, window.stage, window.shader);
		modelview = translate(modelview, -boxes.at(i).getPostion());
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glFlush();
	//glutPostRedisplay(); // FPS
}

Obstacle Object_movement(Obstacle obst, vec3 obj_pos, float force, vec3 direction)
{
	float moved_X = obst.getPostion().x + direction.x * force * 0.01f;
	float moved_Z = obst.getPostion().z + direction.z * force * 0.01f;

	obst.setPosition(vec3(moved_X, ypos, moved_Z));

	float force2 = force;
	force2 -= 0.01f;
	force2 = clamp(force2, 0.0f, 10.0f);
	obst.setForce(force2);

	if (force > 0.0f)
	{
		cout << force << endl;
		cout << "direction " << direction.x << ", " << direction.z << endl;
		cout << "position " << obj_pos.x << ", " << obj_pos.z << endl;
	}

	return obst;
}

//overall display function
void DisplayFunc()
{
	window.FPS++;
	float current_time = (glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	current_time = current_time - window.total_time_paused;
	ostringstream convert;
	convert << (int)current_time;
	string display_current_time = convert.str();

	if (!window.paused)
	{
		glDisable(GL_CULL_FACE);

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, window.size.x, window.size.y);

		mat4 projection;

		if (debug_mode != 3)
			projection = perspective(20.0f, window.aspect, 1.0f, 600.0f);
		else 
			projection = perspective(30.0f, window.aspect, 1.0f, 600.0f);

		radius = 20.1f;

		float tempX = xpos;
		float tempZ = zpos;
		float tempLookatX = lookatX;
		float tempLookatZ = lookatZ;

		mat4 modelview;

		//movement (with a buffer zone in center) in first person view
		if ((debug_mode != 2 && debug_mode != 0) && (xDiffFromCenter > 0.5f || xDiffFromCenter < -0.5f || yDiffFromCenter > 0.5f || yDiffFromCenter < -0.5f))
		{

			if ((xpos <= -105.0f + window.ball_radius || xpos >= 105 - window.ball_radius))	//bounce off walls
			{
				lookatX = xpos - (lookatX - xpos);
				angleV = -angleV;
			}
			if ((zpos <= -105.0f + window.ball_radius  || zpos >= 105.0f - window.ball_radius))	//bounce off walls
			{
				lookatZ = zpos - (lookatZ - zpos);
				while (angleV < 0.0f)
					angleV = 360.0f + angleV;
				angleV = 180 - angleV;
			}


			// turn right or left
			angleV = angleV - xDiffFromCenter * 0.1f;
			lookatX = xpos - sin(angleV * (PI / 180)) * radius ;
			lookatZ = zpos - cos(angleV * (PI / 180)) * radius;

			//move forward and backwards
			xpos = xpos + ((lookatX - xpos) * yDiffFromCenter * 0.02f );	//multiply by level to compensate adding more balls and
			//slowing down of processing them
			zpos = zpos + ((lookatZ - zpos) * yDiffFromCenter * 0.02f);
			ypos = ypos + ((0 - ypos) * 0.003f);

			zpos = clamp(zpos, -104.5f, 104.5f);
			xpos = clamp(xpos, -104.5f, 104.5f);

		}

		//movement in third person view
		if ((debug_mode == 2 || debug_mode == 0) && (xDiffFromCenter > 0.5f || xDiffFromCenter < -0.5f))
		{
			angleH = angleH - xDiffFromCenter * 0.05f;
			cameraX = sin(angleH * (PI / 180)) * 110.0f;
			cameraZ = cos(angleH * (PI / 180)) * 110.0f;

		}

		//check to see if going to hit a box and calculate position
		for (int i = 0; i < (int)boxes.size(); i++)
		{
			if (xpos >= (boxes.at(i).getPostion().x - window.obstacle_width/2.0f - window.ball_radius) 
				&&  xpos <= (boxes.at(i).getPostion().x + window.obstacle_width/2.0f + window.ball_radius))
			{
				if (zpos >= (boxes.at(i).getPostion().z - window.obstacle_width/2.0f - window.ball_radius)
					&& (zpos) <= (boxes.at(i).getPostion().z + window.obstacle_width/2.0f + window.ball_radius))
				{	

					boxes.at(i).setForce(yDiffFromCenter);
					float direction_X; 
					direction_X = boxes.at(i).getPostion().x - xpos;
					float direction_Z; 
					direction_X = boxes.at(i).getPostion().z - zpos;
					boxes.at(i).setDirection(vec3(direction_X, ypos, direction_Z));

					/*cout << boxes.at(i).getPostion().x << ", " << boxes.at(i).getPostion().z << ", " << i << endl;
					cout << "Force: " << boxes.at(i).getForce() << endl;
					cout << "direction: " << boxes.at(i).getDirection().x << ", " << boxes.at(i).getDirection().z << endl;*/

					if (zpos < boxes.at(i).getPostion().z - window.obstacle_width/2.0f || zpos > boxes.at(i).getPostion().z + window.obstacle_width/2.0f)
					{
						//lookatX = tempLookX;
						lookatZ = zpos - (lookatZ - zpos);

						while (angleV < 0.0f)
							angleV = 360.0f + angleV;

						angleV = 180 - angleV;
						cout << "hit z side " << endl;
					}

					else if (xpos < boxes.at(i).getPostion().x - window.obstacle_width/2.0f || xpos > boxes.at(i).getPostion().x + window.obstacle_width/2.0f)
					{
						lookatX = xpos - (lookatX - xpos);
						angleV = -angleV;

						cout << "hit x side " << endl;
					}

				}
			}
			//boxes.at(i) = Object_movement(boxes.at(i), boxes.at(i).getPostion(), boxes.at(i).getForce(), boxes.at(i).getDirection()); 
		}

		//check to see if hitting ball
		for(int i = 0; i < window.num_balls; i++)
		{
			if (xpos  >= (balls.at(i).getPostion().x - window.ball_radius - window.ball_radius) 
				&& (xpos)  <= (balls.at(i).getPostion().x + window.ball_radius + window.ball_radius))
			{
				if (zpos >= (balls.at(i).getPostion().z - window.ball_radius - window.ball_radius) 
					&& (zpos)  <= (balls.at(i).getPostion().z + window.ball_radius + window.ball_radius))
				{	
					if (i == 10)
					{
						window.gameOver = true;
						xpos = tempX;
						zpos = tempZ;
						debug_mode = 2;
					}

					balls.at(i).hit(current_time + 30.0f + (float)window.num_balls * 2.0f, current_time); // set the timer for the ball

					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

					lookatZ = zpos - (lookatZ - zpos);
					lookatX = xpos - (lookatX - xpos);

					//use pythagorean theorem and law of cosines to find angle
					float distanceA = sqrt(pow((balls.at(i).getPostion().x - xpos), 2) + pow((balls.at(i).getPostion().z - zpos), 2));
					float distanceB = sqrt(pow((lookatX - balls.at(i).getPostion().x), 2) + pow((lookatZ - balls.at(i).getPostion().z), 2));
					float distanceC = sqrt(pow((xpos - lookatX), 2) + pow((zpos - lookatZ), 2));

					float angleTemp = acos((pow(distanceB, 2) - pow(distanceA, 2) - pow(distanceC, 2)) / (2 * distanceA * distanceC));

					cout << "original: " << angleV << endl;
					cout << "angleTemp: " << angleTemp << endl;
					if (yDiffFromCenter >= 0.0f)
						angleV = angleV + 90 - angleTemp * 5.0f;
					else
						angleV = angleV - 90 - angleTemp * 5.0f;
					cout << "new angle: " << angleV << endl;
				}
			}
		}


		if (debug_mode == 1)
		{
			modelview = lookAt(vec3(xpos, ypos, zpos), vec3(lookatX, ypos, lookatZ), vec3(0.0f, 1.0f, 0.0f));
		}
		else if (debug_mode == 2)
		{
			modelview = lookAt(vec3(cameraX, 30.0f, cameraZ), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		}
		else if (debug_mode == 3)
		{
			modelview = lookAt(vec3(0.1f, 400.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		}
		else
		{
			modelview = lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(cameraX, 0.0f, cameraZ), vec3(0.0f, 1.0f, 0.0f));
		}

		//FrameBuffering
		RenderIntoFrameBuffer(modelview, projection);
		glViewport(0, 0, window.size.x, window.size.y);

		/*RenderIntoFrameBuffer2(modelview, projection);
		glViewport(0, 0, window.size.x, window.size.y);*/

		glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);

		//Draw skybox
		if (debug_mode != 3)
			sky.Draw(projection, modelview, window.size, window.stage, window.shader);

		//Draw map elements (ground and walls)
		modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
		ground.Draw(projection, modelview, window.size, window.stage, window.shader, current_time);
		walls.Draw_walls(projection, modelview, window.size);
		modelview = translate(modelview, vec3(0.0f, 1.0f, 0.0f));

		//Draw jumbotrons and scoreboards
		modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
		tron.Draw(projection, modelview, window.size, window.stage, window.shader);
		screen.DrawScreen(projection, modelview, window.size, fbo, 0);
		modelview = translate(modelview, vec3(0.0f, 4.5f, -0.2f));
		screen.DrawScreen(projection, modelview, window.size, fbo, 5); //Draw one jumbotron with the field name
		modelview = translate(modelview, vec3(0.0f, -4.5f, 0.2f));
		modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
		modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
		modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
		tron.Draw(projection, modelview, window.size, window.stage, window.shader);
		screen.DrawScreen(projection, modelview, window.size, fbo, 0);
		modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
		modelview = rotate(modelview, 90.0f, vec3(0.0f, 1.0f, 0.0f));
		modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
		/*score.Draw(projection, modelview, window.size, window.stage, window.shader);
		scoreScreen.DrawScreen(projection, modelview, window.size, window.stage, fbo2); */
		tron.Draw(projection, modelview, window.size, window.stage, window.shader);
		screen.DrawScreen(projection, modelview, window.size, fbo, 0);
		modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
		modelview = rotate(modelview, 180.0f, vec3(0.0f, 1.0f, 0.0f));
		modelview = translate(modelview, vec3(0.0f, 0.0f, -107.0f));
		//score.Draw(projection, modelview, window.size, window.stage, window.shader);
		tron.Draw(projection, modelview, window.size, window.stage, window.shader);
		screen.DrawScreen(projection, modelview, window.size, fbo, 0);
		modelview = translate(modelview, vec3(0.0f, 0.0f, 107.0f));
		modelview = rotate(modelview, -90.0f, vec3(0.0f, 1.0f, 0.0f));

		//Draw player (and other objects)
		modelview = translate(modelview, vec3(0.0f, -1.0f, 0.0f));
		modelview = translate(modelview, vec3(0.0f, window.ball_radius, 0.0f));	//make sure balls are on ground
		modelview = translate(modelview, vec3(xpos, ypos, zpos));
		player.Draw(projection, modelview, window.size, window.stage, window.shader);
		modelview = translate(modelview, vec3(-xpos, -ypos, -zpos));

		window.num_hit = 0;
		for (int i = 0; i < (int)balls.size(); i++)	//places the desired number of balls on field
		{
			modelview = translate(modelview, balls.at(i).getPostion());	//places in random location
			if (i == 10)
			{
				bomb.Draw(projection, modelview, window.size, window.stage, window.shader);
			}
			else if (balls.at(i).is_sphere_hit())
			{
				ball2.Draw(projection, modelview, window.size, window.stage, window.shader);
				balls.at(i).setTime(current_time);
				//cout << balls.at(i).getTime()  - balls.at(i).getBirthTime() << endl;
				DisplayTime(modelview, projection,  balls.at(i).getEndTime() - balls.at(i).getTime(), balls.at(i).getPostion());
				window.num_hit++;
			}
			else 
			{
				ball.Draw(projection, modelview, window.size, window.stage, window.shader);
			}
			modelview = translate(modelview, -balls.at(i).getPostion());
		}

		if (window.num_balls - window.num_hit != window.num_remaining)
		{
			window.num_remaining = window.num_balls - window.num_hit;
			ostringstream convert3;
			ostringstream convert2;
			convert3 << window.num_remaining;
			convert2 << window.num_balls;
			displayNumRemaining = convert3.str();
			displayNumBalls = convert2.str();

			window.instructions.pop_back();
			window.instructions.pop_back();
			window.instructions.push_back("Targets remaining: " + displayNumRemaining + " / " + displayNumBalls);
			window.instructions.push_back("Time played: " + display_current_time);

			if (window.num_remaining == 0 )
			{
				ostringstream convert4;
				window.stage++;
				debug_mode = 2;
				window.betweenLevels = true;
				setLevel();
				convert4.clear();
				convert4 << window.stage;
				displayLevel = convert4.str();
				window.instructions.pop_back();
				window.instructions.pop_back();
				window.instructions.pop_back();
				window.instructions.push_back("Level: " + displayLevel);
				window.instructions.push_back("Targets remaining: " + displayNumRemaining + " / " + displayNumBalls);
				window.instructions.push_back("Time played: " + display_current_time);
			}
		}

		modelview = translate(modelview, vec3(0.0f, -window.ball_radius, 0.0f));
		for (int i = 0; i < (int)boxes.size(); i++)	//places the desired number of obstacles on field
		{
			modelview = translate(modelview, boxes.at(i).getPostion());	//places in random location
			box.Draw(projection, modelview, window.size, window.stage, window.shader);
			modelview = translate(modelview, -boxes.at(i).getPostion());
		}

		if (!window.gameStart) //Display start screen
		{
			modelview = rotate(modelview, angleH, vec3(0.0f, 1.0f, 0.0f));
			modelview = scale(modelview, vec3(3.0f, 2.0f, 3.0f));
			screen.DrawScreen(projection, modelview, window.size, fbo, 1); //Display intro screen 
		}

		if (window.betweenLevels) //Display next level screen
		{
			modelview = rotate(modelview, angleH, vec3(0.0f, 1.0f, 0.0f));
			screen.DrawScreen(projection, modelview, window.size, fbo, 2);
		}

		if (window.gameOver) //Display losing screen
		{
			modelview = rotate(modelview, angleH, vec3(0.0f, 1.0f, 0.0f));
			screen.DrawScreen(projection, modelview, window.size, fbo, 3);
		}
		if (window.gameWon)
		{
			modelview = rotate(modelview, angleH, vec3(0.0f, 1.0f, 0.0f));
			screen.DrawScreen(projection, modelview, window.size, fbo, 4);
		}

		window.instructions.pop_back();
		window.instructions.push_back("Time played: " + display_current_time);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DisplayInstructions();

		DisplayCursor();

		//display the minimap
		if (window.minimap)
			DisplayFunc2();

	}

	//glFlush();
	glutSwapBuffers();
	glutPostRedisplay(); // FPS
}


GLvoid TimerFunc(GLint value)	//keeps track of time to be displayed on window
{
	if (window.handle == glutGetWindow())
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
	//GLfloat winX, winY;		//coordinates of the center of screen
	//winX = window.size.x / 2.0f;
	//winY = window.size.y / 2.0f;

	//the cursor's position related to the center of screen
	xDiffFromCenter = (x - window.center.x) / 100.0f;
	yDiffFromCenter = (window.center.y - y) / 100.0f;

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
	//initial values
	debug_mode = 2;
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
	window.obstacle_width = 2.0f;
	window.num_boxes = 5;
	window.stage = 1;
	window.num_remaining = window.num_balls;
	window.gameWon = false;
	window.gameOver = false;
	window.betweenLevels = false;
	window.shader = 0;
	window.minimap = true;

	glutInit(&argc, argv);
	glutInitWindowSize(1300, 960);
	glutInitWindowPosition(500, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);

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

	window.instructions.push_back("Level: 1");
	window.instructions.push_back("Targets remaining: ");
	window.instructions.push_back("Time played: ");

	window.plus_sign.push_back("+");


	assert(GLEW_OK == glewInit());	

	//initialize all objects, and returns error if failed
	if (!ball.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 0, 0.00f))
		return 0;
	if (!ball2.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 1, 0.00f))
		return 0;
	if (!player.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 2, 0.00f))	//hit = 2 to show not a playing object
		return 0;
	if (!bomb.Initialize(window.slices, window.stacks, window.ball_radius, window.shader, 3, 0.00f))
		return 0;
	if (!ground.InitializeFloor(false))
		return 0;
	if (!ground2.InitializeFloor(true))
		return 0;
	if (!walls.InitializeWalls())
		return 0;
	if (!tron.InitializeCylinder())
		return 0;
	if (!screen.InitializeScreen())
		return 0;
	if (!sky.Initialize())
		return 0;
	if (!box.Initialize(window.obstacle_height, window.obstacle_width, window.shader))
		return 0;
	if (!score.Initialize())
		return 0;
	if (!scoreScreen.Initialize())
		return 0;

	if (!fbo.Initialize(glm::ivec2(512, 512), 1, true))
	{
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}
	if (!fbo2.Initialize(glm::ivec2(512, 512), 1, true))
	{
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}

	setLevel();

	//skybox textures
	//assert(TextureManager::Inst()->LoadTexture((const char *) "fulllg.jpg", 1));
	assert(TextureManager::Inst()->LoadTexture((const char *) "sunset.jpg", 1));
	assert(TextureManager::Inst()->LoadTexture((const char *) "cloudy.jpg", 2));
	assert(TextureManager::Inst()->LoadTexture((const char *) "lava.jpg", 3));
	assert(TextureManager::Inst()->LoadTexture((const char *) "interstellar_large.jpg", 4));
	assert(TextureManager::Inst()->LoadTexture((const char *) "simpson_skybox.jpg", 5));
	assert(TextureManager::Inst()->LoadTexture((const char *) "cubemap.jpg", 6));
	assert(TextureManager::Inst()->LoadTexture((const char *) "red_mountain.jpg", 7));
	//assert(TextureManager::Inst()->LoadTexture((const char *) "spheremapgalaxyasteroid.jpg", 7));

	//object textures
	assert(TextureManager::Inst()->LoadTexture((const char *) "crate.jpg", 0));

	//map textures
	assert(TextureManager::Inst()->LoadTexture((const char *) "advertisments.jpg", 8));

	//display screens
	assert(TextureManager::Inst()->LoadTexture((const char *) "introScreen.jpg", 9));
	assert(TextureManager::Inst()->LoadTexture((const char *) "nextLevel.jpg", 10));
	assert(TextureManager::Inst()->LoadTexture((const char *) "you_lost.jpg", 11));
	assert(TextureManager::Inst()->LoadTexture((const char *) "you_won.jpg", 12));

	assert(TextureManager::Inst()->LoadTexture((const char *) "field.jpg", 13));

	glutMainLoop();
	system("pause");
}
