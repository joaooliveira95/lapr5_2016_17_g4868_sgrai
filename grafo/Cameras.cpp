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
