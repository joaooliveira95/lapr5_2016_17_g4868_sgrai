#include "Estado.h"



Estado::Estado()
{
}


Estado::~Estado()
{
}

Cameras Estado::getCamera()
{
	return camera;
}

int Estado::getXMouse()
{
	return xMouse;
}

int Estado::getYMouse()
{
	return yMouse;
}

GLboolean Estado::isLight()
{
	return light;
}

GLboolean Estado::isApresentaNormais()
{
	return apresentaNormais;
}

GLint Estado::getLightViewer()
{
	return lightViewer;
}

GLint Estado::getEixoTranslaccao()
{
	return eixoTranslaccao;
}

GLdouble Estado::getEixo(int n)
{
	return eixo[n];
}

void Estado::setCamera(Cameras c)
{
	camera = c;
}

void Estado::setXMouse(int x)
{
	xMouse = x;
}

void Estado::setYMouse(int y)
{
	yMouse = y;
}

void Estado::setLight(GLboolean l)
{
	light = l;
}

void Estado::setApresentaNormais(GLboolean n)
{
	apresentaNormais = n;
}

void Estado::setLightViewer(GLint l)
{
	lightViewer = l;
}

void Estado::setEixoTranslaccao(GLint e)
{
	eixoTranslaccao = e;
}

void Estado::setEixo(int n, GLdouble val)
{
	eixo[n] = val;
}
