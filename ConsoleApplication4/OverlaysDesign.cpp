﻿#include "OverlaysDesign.h"

#define HEALTH_BAR_HEIGHT 20

OverlaysDesign::OverlaysDesign()
{
}


OverlaysDesign::~OverlaysDesign()
{
}


void OverlaysDesign::bitmapString(char *str, double x, double y) {
	int i, n;

	n = strlen(str);
	glRasterPos2d(x, y);
	for (i = 0; i<n; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)str[i]);
}

void OverlaysDesign::bitmapCenterString(char *str, double x, double y) {
	int i, n;
	n = strlen(str);
	glRasterPos2d(x - glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)str)*0.5, y);
	for (i = 0; i<n; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)str[i]);
}


void OverlaysDesign::desenhaKm(float x) {
	char str[255];
	

	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glLineWidth(3);
	glLineStipple(2, 0xFF00);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 20);
	glDisable(GL_DEPTH_TEST);
	glColor3f(2, 2, 2);

	sprintf_s(str, 255, "%.f Km", x);
	bitmapCenterString(str, width*0.8, height*0.9);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void OverlaysDesign::desenhaTempo(int seg,int min) {
	char str[255];


	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glLineWidth(3);
	glLineStipple(2, 0xFF00);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 20);
	glDisable(GL_DEPTH_TEST);
	glColor3f(2, 2, 2);

	sprintf_s(str, 255, "%d : %d", seg, min);
	bitmapCenterString(str, width*0.1, height*0.9);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}
