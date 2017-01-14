#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <iostream>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include "mathlib.h"
#include "TextureLoader.h"
#include "studio.h"
#include "mdlviewer.h"
#include "mathlib.h"
#include "Import3DS\Model_3DS.h"
#include "Weather.h"
#include <AL\alut.h>

#define OBJECTO_RAIO		    0.12
#define OBJECTO_ROTACAO			2
#define	OBJECTO_ALTURA			0.5
#define OBJECTO_VELOCIDADE		2.5
#define SCALE_POI			    0.002
#define PREDIO1_POI			    0.002
#define PONTE_POI				0.008
#define ESCOLA_POI				0.002
#define COMBOIO_POI				0.006
#define FABRICA_POI				0.005
#define MAGAZIN_POI				0.008
#define SCALE_HOMER			    0.005
#define ALTURA_HOMER			SCALE_HOMER*100
#define GAP						25

#define PREDIO_ID				0
#define PONTE_ID				1
#define ESCOLA_ID				2
#define COMBOIO_ID				3
#define FABRICA_ID				4
#define MAGAZIN_ID				5
#define STOP_ID					6
#define CANDEEIRO_ID			7
#define NUM_MODELS_POIS			8


#define EIXO_X		1
#define EIXO_Y		2
#define EIXO_Z		3

using namespace std;

const GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat green_light[] = { 0.0, 1.0, 0.0, 1.0 };
const GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
const GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

//#define SCALE_HOMER				0.01

// luzes e materiais

const GLfloat mat_ambient[][4] = { { 0.33f, 0.22f, 0.03f, 1.0f },	// brass
{ 0.0f, 0.0f, 0.0f },			// red plastic
{ 1, 1, 1 },	// emerald
{ 0.02f, 0.02f, 0.02f },		// slate
{ 0.0f, 0.0f, 0.1745f },		// azul
{ 0.02f, 0.02f, 0.02f },		// preto
{ 0.1745f, 0.1745f, 0.1745f } };// cinza


const GLfloat mat_diffuse[][4] = { { 0.78f, 0.57f, 0.11f, 1.0f },		// brass
{ 0.5f, 0.0f, 0.0f },				// red plastic
{ 0.07568f, 0.07568f, 0.07568f },	// emerald
{ 0.78f, 0.78f, 0.78f },			// slate
{ 0.0f, 0.0f,  0.61424f },			// azul
{ 0.08f, 0.08f, 0.08f },			// preto
{ 0.61424f, 0.61424f, 0.61424f } };	// cinza

const GLfloat mat_specular[][4] = { { 0.99f, 0.91f, 0.81f, 1.0f },			// brass
{ 0.7f, 0.6f, 0.6f },					// red plastic
{ 0.633f,  0.633f, 0.633f },		// emerald
{ 0.14f, 0.14f, 0.14f },				// slate
{ 0.0f, 0.0f, 0.727811f },			// azul
{ 0.03f, 0.03f, 0.03f },				// preto
{ 0.727811f, 0.727811f, 0.727811f } };	// cinza

const GLfloat mat_shininess[] = { 27.8f,	// brass
32.0f,	// red plastic
76.8f,	// emerald
18.78f,	// slate
30.0f,	// azul
75.0f,	// preto
60.0f };	// cinza

enum tipo_material { brass, red_plastic, emerald, slate, azul, preto, cinza };

#ifdef __cplusplus
inline tipo_material operator++(tipo_material &rs, int) {
	return rs = (tipo_material)(rs + 1);
}
#endif

typedef	GLdouble Vertice[3];
typedef	GLdouble Vector[4];

typedef struct {
	ALuint buffer, source;
	ALboolean tecla_s;
} Som;


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
	Som musica;
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

typedef struct Poi {
	StudioModel  model;
	string		nome;
	GLfloat		scale;
	GLfloat altitude;
};


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
	Poi   poi[NUM_MODELS_POIS];
	GLboolean     andar;
	GLuint        prev;
	float			  km;
	int			  tempo;
	//Model_3DS m3ds;
}Modelo;

typedef struct Texture {
	glTexture chao;
	glTexture rotunda;
	glTexture rotundaVisita;
	glTexture info;
	glTexture banda;
}Texture;

typedef struct InfoNo {
	int i;
	GLboolean isActive;
}InfoNo;


typedef struct InfoArco {
	string nome;
	GLboolean isActive;
}InfoArco;


