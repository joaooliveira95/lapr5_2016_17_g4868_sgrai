#pragma once
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <iostream>
#include <math.h>
#include <time.h>

#include <GL\glut.h>
#include <GL/GLAux.h>
#include "Grafo.h"
#include "TextureLoader.h"



#define graus(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)
#define K_LIGACAO				1.1

class DrawGraph
{
public:
	DrawGraph();
	~DrawGraph();
	//void desenhaArco(Ligacao arco, glTexture textura);
	void desenhaNo(Ponto noi, glTexture textura);

//	void desenhaElemLigaInicial(Ligacao arco, glTexture textura, StudioModel stop);
	void desenhaElemLigaFinal(Ligacao arco, glTexture textura);
	void desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, glTexture textura);
	void desenhaChaoRedondo(float largura, GLfloat x0, GLfloat y0, GLfloat z, glTexture textura);
	void desenhaSolo();

private:
	void CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[]);
	GLdouble  VectorNormalize(GLdouble v[]);
};

