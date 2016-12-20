#pragma once
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>
#include "Lights.h"

typedef	GLdouble Vertice[3];

class Cameras{
	GLfloat fov;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	Vertice center;

public:
	Cameras();
	~Cameras();

	void setCamera();//GLboolean isLight, Lights lights, GLfloat g_pos_luz1[3], GLfloat g_pos_luz2[3]

	GLfloat getFov();
	GLdouble getDir_Lat();
	GLdouble getDir_Long();
	GLfloat getDist();
	GLdouble getCenter(int n);

	void setFov(GLfloat fov);
	void setDir_Lat(GLdouble dir_lat);
	void setDir_Long(GLdouble dir_long);
	void setDist(GLfloat dist);
	void setCenter(int n, GLdouble val);

};

