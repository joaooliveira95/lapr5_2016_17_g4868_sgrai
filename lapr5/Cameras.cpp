#include "Cameras.h"

Cameras::Cameras()
{
}


Cameras::~Cameras()
{
}

GLfloat Cameras::getFov()
{
	return fov;
}

GLdouble Cameras::getDir_Lat() {
	return dir_lat;
}

GLdouble Cameras::getDir_Long()
{
	return dir_long;
}

GLfloat Cameras::getDist()
{
	return dist;
}

GLdouble Cameras::getCenter(int n)
{
	return center[n];
}

void Cameras::setFov(GLfloat f)
{
	fov = f;
}

void Cameras::setDir_Lat(GLdouble dlat)
{
	dir_lat = dlat;
}

void Cameras::setDir_Long(GLdouble dlong)
{
	dir_long = dlong;
}

void Cameras::setDist(GLfloat d)
{
	dist = d;
}

void Cameras::setCenter(int n, GLdouble val)
{
	center[n] = val;
}

void Cameras:: setCamera() {//GLboolean isLight, Lights lights, GLfloat g_pos_luz1[3], GLfloat g_pos_luz2[3]
	Vertice eye;

	eye[0] = getCenter(0) + getDist()*cos(getDir_Long())*cos(getDir_Lat());
	eye[1] = getCenter(1) + getDist()*sin(getDir_Long())*cos(getDir_Lat());
	eye[2] = getCenter(2) + getDist()*sin(getDir_Lat());

	/*if (isLight) {*/
		gluLookAt(eye[0], eye[1], eye[2], getCenter(0), getCenter(1), getCenter(2), 0, 0, 1);
		/*lights.putLights((GLfloat*)white_light, g_pos_luz1, g_pos_luz2);
	}
	else {
		lights.putLights((GLfloat*)white_light,g_pos_luz1,g_pos_luz2);
		gluLookAt(eye[0], eye[1], eye[2], getCenter(0), getCenter(1), getCenter(2), 0, 0, 1);
	}*/
}