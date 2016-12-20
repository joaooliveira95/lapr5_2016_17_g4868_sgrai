#include <math.h>
#include <stdlib.h>     
#include <stdio.h>
#include <GL\glut.h>
#include <iostream>
#include "Materials.h"
#include "grafos.h"

#pragma once

#define EIXO_X		1
#define EIXO_Y		2
#define EIXO_Z		3

class DesenhaLabirinto
{
public:
	DesenhaLabirinto();
	~DesenhaLabirinto();

	void desenhaSolo();
	void desenhaLabirinto(int numNos, int numArcos, GLboolean isApresentaNormais);
	void desenhaEixos(GLdouble eixo0, GLdouble eixo1, GLdouble eixo2, GLUquadric *quad);
	void desenhaEixo(GLUquadric *quad);

	void desenhaArco(Arco arco, GLboolean isApresentaNormais);
	void desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, int orient, GLboolean isApresentaNormais);
	void desenhaParede(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, GLboolean isApresentaNormais);
	void desenhaNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], tipo_material mat);
	void desenhaNo(int no, GLboolean isApresentaNormais);
	void CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[]);
	GLdouble VectorNormalize(GLdouble v[]);
};

