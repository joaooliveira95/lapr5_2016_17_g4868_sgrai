#pragma once
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>

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

