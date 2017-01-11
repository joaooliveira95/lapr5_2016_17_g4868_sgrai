#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <iostream>
#include <math.h>
#include <time.h>
#include <GL\glut.h>
#include <GL/GLAux.h>
#include "mathlib.h"
#include "TextureLoader.h"
#include "studio.h"
#include "mdlviewer.h"
#include "mathlib.h"

#define OBJECTO_RAIO		    0.12
#define OBJECTO_ROTACAO			2
#define	OBJECTO_ALTURA			0.5
#define OBJECTO_VELOCIDADE		2.5
#define SCALE_POI			    0.002
#define SCALE_HOMER			    0.005
#define GAP						25

//#define SCALE_HOMER				0.01

// luzes e materiais

const GLfloat mat_ambient[][4] = { { 0.33, 0.22, 0.03, 1.0 },	// brass
{ 0.0, 0.0, 0.0 },			// red plastic
{ 0.0215, 0.1745, 0.0215 },	// emerald
{ 0.02, 0.02, 0.02 },		// slate
{ 0.0, 0.0, 0.1745 },		// azul
{ 0.02, 0.02, 0.02 },		// preto
{ 0.1745, 0.1745, 0.1745 } };// cinza

const GLfloat mat_diffuse[][4] = { { 0.78, 0.57, 0.11, 1.0 },		// brass
{ 0.5, 0.0, 0.0 },				// red plastic
{ 0.07568, 0.61424, 0.07568 },	// emerald
{ 0.78, 0.78, 0.78 },			// slate
{ 0.0, 0.0,  0.61424 },			// azul
{ 0.08, 0.08, 0.08 },			// preto
{ 0.61424, 0.61424, 0.61424 },
{ 0.0, 0.0, 0.0 } };	// branco

const GLfloat mat_specular[][4] = { { 0.99, 0.91, 0.81, 1.0 },			// brass
{ 0.7, 0.6, 0.6 },					// red plastic
{ 0.633, 0.727811, 0.633 },		// emerald
{ 0.14, 0.14, 0.14 },				// slate
{ 0.0, 0.0, 0.727811 },			// azul
{ 0.03, 0.03, 0.03 },				// preto
{ 0.727811, 0.727811, 0.727811 },
{ 0.0, 0.0, 0.0 } };	// branco

const GLfloat mat_shininess[] = { 27.8,	// brass
32.0,	// red plastic
76.8,	// emerald
18.78,	// slate
30.0,	// azul
75.0,	// preto
60.0,
10.0 };	// cinza

enum tipo_material { brass, red_plastic, emerald, slate, azul, preto, cinza, white };

#ifdef __cplusplus
inline tipo_material operator++(tipo_material &rs, int) {
	return rs = (tipo_material)(rs + 1);
}
#endif

typedef	GLdouble Vertice[3];
typedef	GLdouble Vector[4];

typedef struct teclas_t {
	GLboolean   up, down, left, right;
}teclas_t;

typedef struct Camera {
	GLfloat fov;
	GLdouble dir_lat;
	GLdouble dir_long;
	GLfloat dist;
	Vertice center;

}Camera;

typedef struct Estado {
	teclas_t      teclas;
	Camera		camera;
	GLboolean  isFP;
	GLint		timer;
	int			xMouse, yMouse;
	GLint		mainWindow, topSubwindow, navigateSubwindow, menuSubwindow;
	GLboolean	light;
	GLboolean	apresentaNormais;
	GLint		lightViewer;
	GLint		eixoTranslaccao;
	GLdouble	eixo[3];
}Estado;

typedef struct pos_t {
	GLfloat		x, y, z;
} pos_t;

typedef struct objecto_t {
	pos_t		pos;
	GLfloat		dir;
	GLfloat		vel;
	GLfloat		andar;
} objecto_t;

typedef struct Modelo {
#ifdef __cplusplus
	tipo_material cor_cubo;
#else
	enum tipo_material cor_cubo;
#endif

	GLfloat g_pos_luz1[4];
	GLfloat g_pos_luz2[4];

	GLfloat escala;
	GLUquadric *quad;
	objecto_t	  objecto;
	GLuint        xMouse;
	GLuint        yMouse;
	StudioModel   personagem;
	StudioModel   poi;
	GLboolean     andar;
	GLuint        prev;
	float			  km;
	int			  tempo;
}Modelo;

typedef struct Texture {
	glTexture chao;
	glTexture rotunda;
	glTexture info;
}Texture;

typedef struct InfoNo {
	int i;
	GLboolean isActive;
}InfoNo;
