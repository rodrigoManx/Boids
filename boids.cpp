#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "quadtree.h"

using namespace std;

#define ECHAP 27

#define AMOUNT 10
int timee = 0;
int timebase = 0;
float X = 0.0;
float Z = 0.0;
float RXP = 0.0;
float RYP = 0.0;
float DX = 0.0;
float DY = 0.0;

vector3 limits(183	, 100, 100);
vector3 center(0, 0, 0);
float visionRadius = 15;
float personalSpaceR = 7.5;
float vLimit = 10;
float size = 0.75;
int regions = 1;
int boidsAmount = 1000;
int predatorsAmount = 1;
quadTree3d *jail;

GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		Z = Z + 1;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_DOWN:
		Z = Z - 1;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_LEFT:					
		X = X + 1;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_RIGHT:				
		X = X - 1;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;
	}
}
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		RXP = x;
		RYP = y;
	}
}

GLvoid callback_motion(int x, int y)
{
	DX += x - RXP;
	DY += y - RYP;
	RXP = x;
	RYP = y;
	glutPostRedisplay();						
}
void window_reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 400.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void window_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 280.0, 0.0, 0.0, 0.0, 0,1,0);

	timee = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	float dt = float(timee -timebase) / 1000;// delta time
	timebase = timee;

	glTranslatef(X, 0.0, Z);
	glRotatef(DX, 0.0, cos((DY / 180) * PI) * 1.0, sin((DY / 180 * PI)) * 1.0);
	//glRotatef(DX, 0.0, 0.0, 1.0);
	glRotatef(DY, 1.0, 0.0, 0.0);

	jail->draw(regions);
	jail->foo(dt);
	
	glutSwapBuffers();
	glFlush();
}

void initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);

	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 1);
}

void window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		delete jail;
		break;

	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


int main(int argc, char **argv)
{	
	srand(time(NULL));
	jail = new quadTree3d(limits, center, boidsAmount, visionRadius, personalSpaceR, vLimit, predatorsAmount, size);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(500, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("TP 2 : Transformaciones");
	initGL();
	glutDisplayFunc(window_display);
	glutReshapeFunc(window_reshape);
	glutKeyboardFunc(window_key);
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);
	glutSpecialFunc(&callback_special);
	glutIdleFunc(window_display);
	glutMainLoop();

	return 0;
}