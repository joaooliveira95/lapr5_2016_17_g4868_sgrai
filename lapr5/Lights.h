#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#pragma once

const GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat green_light[] = { 0.0, 1.0, 0.0, 1.0 };
const GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
const GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

class Lights
{
public:
	Lights();
	~Lights();
	void putLights(GLfloat* diffuse, GLfloat g_pos_luz1[3], GLfloat g_pos_luz2[3]);
};

