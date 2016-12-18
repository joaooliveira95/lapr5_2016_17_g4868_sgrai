#pragma once
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include "Cameras.h"

class Estado{
	Cameras		camera;
	int			xMouse, yMouse;
	GLboolean	light;
	GLboolean	apresentaNormais;
	GLint		lightViewer;
	GLint		eixoTranslaccao;
	GLdouble	eixo[3];

public:
	Estado();
	~Estado();
	
	Cameras getCamera();
	int getXMouse();
	int getYMouse();
	GLboolean isLight();
	GLboolean isApresentaNormais();
	GLint getLightViewer();
	GLint getEixoTranslaccao();
	GLdouble getEixo(int n);

	void setCamera(Cameras c);
	void setXMouse(int x);
	void setYMouse(int y);
	void setLight(GLboolean l);
	void setApresentaNormais(GLboolean n);
	void setLightViewer(GLint l);
	void setEixoTranslaccao(GLint e);
	void setEixo(int n, GLdouble val);

};

