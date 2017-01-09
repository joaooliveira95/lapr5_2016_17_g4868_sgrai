#define _USE_MATH_DEFINES
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <iostream>
#include <math.h>
#include <time.h>
#include <GL\glut.h>
#include <GL/GLAux.h>
#include "mathlib.h"
#include "studio.h"
#include "mdlviewer.h"
#include "grafos.h"
#include "OverlaysDesign.h"
#include "Skybox.h"
#include "ConsolaMenu.h"
#include "TextureLoader.h"

using namespace std;

#define graus(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)
#define OBJECTO_RAIO		    0.12
#define OBJECTO_ROTACAO			2
#define	OBJECTO_ALTURA			0.5
#define OBJECTO_VELOCIDADE		2.5
#define SCALE_POI			    0.04
#define SCALE_HOMER			    0.005
#define K_LIGACAO				1.1
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
{ 0.61424, 0.61424, 0.61424 } };	// cinza

const GLfloat mat_specular[][4] = { { 0.99, 0.91, 0.81, 1.0 },			// brass
{ 0.7, 0.6, 0.6 },					// red plastic
{ 0.633, 0.727811, 0.633 },		// emerald
{ 0.14, 0.14, 0.14 },				// slate
{ 0.0, 0.0, 0.727811 },			// azul
{ 0.03, 0.03, 0.03 },				// preto
{ 0.727811, 0.727811, 0.727811 } };	// cinza

const GLfloat mat_shininess[] = { 27.8,	// brass
32.0,	// red plastic
76.8,	// emerald
18.78,	// slate
30.0,	// azul
75.0,	// preto
60.0 };	// cinza

enum tipo_material { brass, red_plastic, emerald, slate, azul, preto, cinza };

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
}Texture;

Estado estado;
Modelo modelo;
SKYBOX * skybox;
Texture textures;

GLfloat nextZ;

TextureLoader tl = TextureLoader();




void initEstado() {
	estado.isFP = GL_FALSE;
	estado.camera.dir_lat = M_PI / 6;
	estado.camera.dir_long = -M_PI/2;
	estado.camera.fov = 60;
	estado.camera.dist = 5;
	estado.eixo[0] = 0;
	estado.eixo[1] = 0;
	estado.eixo[2] = 0;
	estado.camera.center[0] = modelo.objecto.pos.x;
	estado.camera.center[1] = modelo.objecto.pos.y;
	estado.camera.center[2] = modelo.objecto.pos.z;


	estado.light = GL_FALSE;
	estado.apresentaNormais = GL_FALSE;
	estado.lightViewer = 1;
}

void initModelo() {
	modelo.escala = 0.2;

	modelo.cor_cubo = brass;
	modelo.g_pos_luz1[0] = -5.0;
	modelo.g_pos_luz1[1] = 5.0;
	modelo.g_pos_luz1[2] = 5.0;
	modelo.g_pos_luz1[3] = 0.0;
	modelo.g_pos_luz2[0] = 5.0;
	modelo.g_pos_luz2[1] = -15.0;
	modelo.g_pos_luz2[2] = 5.0;
	modelo.g_pos_luz2[3] = 0.0;
}


void myInit(){
	modelo.objecto.pos.x = 0;
	modelo.objecto.pos.y = -50;
	modelo.objecto.pos.z = 2.5;
	
	modelo.objecto.dir = 0;
	modelo.objecto.vel = OBJECTO_VELOCIDADE;
	modelo.andar = GL_FALSE;

	time_t timer = time(0);

	modelo.tempo = timer;


	estado.timer = 100;
	GLfloat LuzAmbiente[] = { 0.5,0.5,0.5, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.lightViewer);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	initModelo();
	initEstado();
	modelo.quad = gluNewQuadric();
	gluQuadricDrawStyle(modelo.quad, GLU_FILL);
	gluQuadricNormals(modelo.quad, GLU_OUTSIDE);	
	
	tl.LoadTextureFromDisk("calcada.jpg", &textures.chao);
	tl.LoadTextureFromDisk("rotunda.jpg", &textures.rotunda);

	leGrafo();
}



void material(enum tipo_material mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[mat]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[mat]);
}


const GLfloat red_light[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat green_light[] = { 0.0, 1.0, 0.0, 1.0 };
const GLfloat blue_light[] = { 0.0, 0.0, 1.0, 1.0 };
const GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };


void putLights(GLfloat* diffuse)
{
	const GLfloat white_amb[] = { 0.2, 0.2, 0.2, 1.0 };
	const GLfloat dif2[] = { 0.0, 0.0, 0.8, 1.0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, modelo.g_pos_luz1);

	/* desenhar luz */
	material(red_plastic);
	glPushMatrix();
	glTranslatef(modelo.g_pos_luz1[0], modelo.g_pos_luz1[1], modelo.g_pos_luz1[2]);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidCube(0.1);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(modelo.g_pos_luz2[0], modelo.g_pos_luz2[1], modelo.g_pos_luz2[2]);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidCube(0.1);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}


void setCamera() {
	Vertice eye;
	/*eye[0] = modelo.objecto.pos.x;
	eye[1] = modelo.objecto.pos.y-2;
	eye[2] = modelo.objecto.pos.z;*/

	if (estado.isFP) {
		eye[0] = modelo.objecto.pos.x;
		eye[1] = modelo.objecto.pos.y;
		eye[2] = modelo.objecto.pos.z + 1.25;

		estado.camera.center[0] = modelo.objecto.pos.x - sin(modelo.objecto.dir)*(0.25);
		estado.camera.center[1] = modelo.objecto.pos.y + cos(modelo.objecto.dir)*(0.25);
		estado.camera.center[2] = modelo.objecto.pos.z + 1.25;

		if (estado.light) {
			gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
			putLights((GLfloat*)white_light);
		}
		else {
			putLights((GLfloat*)white_light);
			gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
		}
	}else {
		estado.camera.center[0] = modelo.objecto.pos.x;
		estado.camera.center[1] = modelo.objecto.pos.y;
		estado.camera.center[2] = modelo.objecto.pos.z;

		eye[0] = estado.camera.center[0] + estado.camera.dist*cos(estado.camera.dir_long)*cos(estado.camera.dir_lat);
		eye[1] = estado.camera.center[1] + estado.camera.dist*sin(estado.camera.dir_long)*cos(estado.camera.dir_lat);
		eye[2] = estado.camera.center[2] + estado.camera.dist*sin(estado.camera.dir_lat);

		if (estado.light) {
			gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
			putLights((GLfloat*)white_light);
		}
		else {
			putLights((GLfloat*)white_light);
			gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
		}
	}
}


void myReshapeMainWindow(int width, int height){
	
		GLint w, h;
		w = (width - GAP * 3)*.5;
		h = (height - GAP * 2);
		glutSetWindow(estado.topSubwindow);
		glutPositionWindow(GAP+5, height -(GAP+h/3+5));
		glutReshapeWindow(w/3, h/3);

		glutSetWindow(estado.navigateSubwindow);
		glutPositionWindow( GAP, GAP);
		glutReshapeWindow(width - 2*GAP, height - 2*GAP);

}



void myReshapeTopWindow(int width, int height) {
	// glViewport(botom, left, width, height)
	// define parte da janela a ser utilizada pelo OpenGL
	glViewport(0, 0, (GLint)width, (GLint)height);
	// Matriz Projeccao
	// Matriz onde se define como o mundo e apresentado na janela
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width / height, .5, 100);
	// Matriz Modelview
	// Matriz onde são realizadas as tranformacoes dos modelos desenhados
	glMatrixMode(GL_MODELVIEW);
}

void redisplayAll(void)
{
	glutSetWindow(estado.mainWindow);
	glutPostRedisplay();
	glutSetWindow(estado.topSubwindow);
	glutPostRedisplay();
	glutSetWindow(estado.navigateSubwindow);
	glutPostRedisplay();
	glutSetWindow(estado.menuSubwindow);
	glutPostRedisplay();

}

void displayMainWindow(){
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glutSwapBuffers();
}

GLboolean inAreaNo(No no, GLfloat ny, GLfloat nx) {
	float largura =no.largura;
	float x = no.x;
	float y = no.y;
	float z = no.z;
	
	//Valida se o Homer está dentro do nó de raio = largura, centrado em (X,y)
	//Se estiver devolve True e atualiza nextZ ( altura do Homer)
	if(pow(nx - x,2) + pow(ny-y,2) <= pow(largura,2)){
		modelo.objecto.pos.y = ny;
		modelo.objecto.pos.x = nx;
		modelo.objecto.pos.z = z + OBJECTO_ALTURA/2;
		return true;
	}

	return false;
}

//Valida se o Homer esta dentro de um arco
GLboolean inAreaArco(Arco arco, GLfloat x1P, GLfloat y1P) {
	No *noi, *nof;
	noi = &nos[arco.noi];
	nof = &nos[arco.nof];

	float si = K_LIGACAO * noi->largura;
	float sf = K_LIGACAO * nof->largura;

	float xi = noi->x;
	float xf = nof->x;
	float yi = noi->y;
	float yf = nof->y;
	float zi = noi->z;

	float orientacao_a = atan2f((yf - yi), (xf - xi));
	float comp_p = sqrt(pow(xf - xi, 2) + pow(yf - yi, 2)) - si - sf;
	float desnivel_h = nof->z - noi->z;


	float x2P = (x1P - xi) * cos(orientacao_a) + (y1P - yi) * sin(orientacao_a);
	float y2P = (y1P - yi) * cos(orientacao_a) - (x1P - xi) * sin(orientacao_a);

	float largura = arco.largura / 2.0;
	if (si <= x2P && x2P <= si+comp_p) {
		if (-largura <= y2P && y2P <= largura) {
			modelo.objecto.pos.x = x1P;
			modelo.objecto.pos.y = y1P;
			modelo.objecto.pos.z = zi + (x2P-si) / comp_p * desnivel_h + OBJECTO_ALTURA / 2;
			return true;
		}
	}
	return false;
}

boolean inAreaElemLiga(Arco arco, GLfloat x1P, GLfloat y1P) {
	No *noi, *nof;
	noi = &nos[arco.noi];
	nof = &nos[arco.nof];

	float si = K_LIGACAO * noi->largura;

	float xi = noi->x;
	float xf = nof->x;
	float yi = noi->y;
	float yf = nof->y;
	float zi = noi->z;

	float orientacao_a = atan2f((yf - yi), (xf - xi));

	float x2P = (x1P - xi) * cos(orientacao_a) + (y1P - yi) * sin(orientacao_a);
	float y2P = (y1P - yi) * cos(orientacao_a) - (x1P - xi) * sin(orientacao_a);

	float largura = arco.largura / 2.0;
	if (0.0 <= x2P && x2P <= si) {
		if (-largura <= y2P && y2P <= largura) {
			modelo.objecto.pos.x = x1P;
			modelo.objecto.pos.y = y1P;
			modelo.objecto.pos.z = zi + OBJECTO_ALTURA / 2;
			return true;
		}
	}
	return false;
}

boolean inAreaElemLigaFinal(Arco arco, GLfloat x1P, GLfloat y1P) {
	No *noi, *nof;
	noi = &nos[arco.nof];
	nof = &nos[arco.noi];

	float si = K_LIGACAO * noi->largura;

	float xi = noi->x;
	float xf = nof->x;
	float yi = noi->y;
	float yf = nof->y;
	float zi = noi->z;

	float orientacao_a = atan2f((yf - yi), (xf - xi));

	float x2P = (x1P - xi) * cos(orientacao_a) + (y1P - yi) * sin(orientacao_a);
	float y2P = (y1P - yi) * cos(orientacao_a) - (x1P - xi) * sin(orientacao_a);

	float largura = arco.largura / 2.0;
	if (0.0 <= x2P && x2P <= si) {
		if (-largura <= y2P && y2P <= largura) {
			modelo.objecto.pos.x = x1P;
			modelo.objecto.pos.y = y1P;
			modelo.objecto.pos.z = zi + OBJECTO_ALTURA / 2;
			return true;
		}
	}
	return false;
}


//Valida se o Homer se encontra fora das áreas permitidas
GLboolean detectaColisao(GLfloat nx, GLfloat nz) {
	for (int i = 0; i < numNos; i++) {
		if (inAreaNo(nos[i], nx, nz)) {
			return(GL_FALSE);
		}
	}

	for (int i = 0; i < numArcos; i++) {
		if (inAreaArco(arcos[i], nz, nx)) {
			return(GL_FALSE);
		}
		if (inAreaElemLiga(arcos[i], nz, nx)) {
			return (GL_FALSE);
		}
		if (inAreaElemLigaFinal(arcos[i], nz, nx)) {
			return (GL_FALSE);
		}
	}

	if (modelo.personagem.GetSequence() != 20)
	{
		modelo.personagem.SetSequence(20);
		modelo.personagem.SetSequence(20);
	}
	return(GL_TRUE);
}


void teclaUp(float velocidade) {
	GLfloat ny, nx;
	ny = modelo.objecto.pos.y + cos(modelo.objecto.dir)*velocidade;
	nx = modelo.objecto.pos.x + sin(-modelo.objecto.dir)*velocidade;
	if (!detectaColisao(ny + cos(modelo.objecto.dir)*OBJECTO_RAIO, nx + sin(-modelo.objecto.dir)*OBJECTO_RAIO) &&
		!detectaColisao(ny + cos(modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO, nx + sin(-modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO) &&
		!detectaColisao(ny + cos(modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO, nx + sin(-modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO)) {


		estado.camera.center[0] = modelo.objecto.pos.x;
		estado.camera.center[1] = modelo.objecto.pos.y;
		estado.camera.center[2] = modelo.objecto.pos.z;

		modelo.km += 0.5;

	}
}

void teclaDown(float velocidade) {
	GLfloat ny, nx;
	ny = modelo.objecto.pos.y - cos(modelo.objecto.dir)*velocidade;
	nx = modelo.objecto.pos.x - sin(-modelo.objecto.dir)*velocidade;
	if (!detectaColisao(ny, nx) &&
		!detectaColisao(ny - cos(modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO, nx - sin(-modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO) &&
		!detectaColisao(ny - cos(modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO, nx - sin(-modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO)) {

		estado.camera.center[0] = modelo.objecto.pos.x;
		estado.camera.center[1] = modelo.objecto.pos.y;
		estado.camera.center[2] = modelo.objecto.pos.z;

		modelo.km += 0.5;
	}
	
}

void timer(int value) {
	GLboolean andar = GL_FALSE;
	
	GLuint curr = GetTickCount();
	float velocidade = modelo.objecto.vel*(curr - modelo.prev)*0.001;
	
	//animacao colisao
	if (modelo.personagem.GetSequence() != 20){
		glutTimerFunc(estado.timer, timer, 0);
	}else if (value<4500){
		glutTimerFunc(estado.timer, timer, value + curr - modelo.prev);
		glutPostRedisplay();
		return;
	}else{
		modelo.personagem.SetSequence(0);
		modelo.personagem.SetSequence(0);
		glutTimerFunc(estado.timer, timer, 0);
	}

	modelo.prev = curr;

	if (estado.teclas.up) {
		teclaUp(velocidade);
		andar = GL_TRUE;
	}
	if (estado.teclas.down) {
		teclaDown(velocidade);
		andar = GL_TRUE;
	}
	
	if (estado.teclas.left) {
		modelo.objecto.dir += rad(OBJECTO_ROTACAO);
		estado.camera.dir_long += rad(OBJECTO_ROTACAO);
	}
	if (estado.teclas.right) {
		modelo.objecto.dir -= rad(OBJECTO_ROTACAO);
		estado.camera.dir_long -= rad(OBJECTO_ROTACAO);
	}

	if (estado.isFP) {
		setCamera();
	}

	if (modelo.personagem.GetSequence() != 20)
	{
		if (andar && modelo.personagem.GetSequence() != 3)
		{
			modelo.personagem.SetSequence(3);
		//	modelo.homer[JANELA_TOP].SetSequence(3);
		}
		else
			if (!andar && modelo.personagem.GetSequence() != 0)
			{
				modelo.personagem.SetSequence(0);
			//	modelo.homer[JANELA_TOP].SetSequence(0);
			}
	}


	redisplayAll();
}

void imprime_ajuda(void){
	printf("\n\nDesenho de um labirinto a partir de um grafo\n");
	printf("h,H - Ajuda \n");
	printf("i,I - Reset dos Valores \n");
	printf("******* Diversos ******* \n");
	printf("l,L - Alterna o calculo luz entre Z e eye (GL_LIGHT_MODEL_LOCAL_VIEWER)\n");
	printf("k,K - Alerna luz de camera com luz global \n");
	printf("s,S - PolygonMode Fill \n");
	printf("w,W - PolygonMode Wireframe \n");
	printf("p,P - PolygonMode Point \n");
	printf("c,C - Liga/Desliga Cull Face \n");
	printf("n,N - Liga/Desliga apresentação das normais \n");
	printf("******* grafos ******* \n");
	printf("F1  - Grava grafo do ficheiro \n");
	printf("F2  - Lê grafo para ficheiro \n");
	printf("F6  - Cria novo grafo\n");
	printf("******* Camera ******* \n");
	printf("Botão esquerdo - Arrastar os eixos (centro da camera)\n");
	printf("Botão direito  - Rodar camera\n");
	printf("Botão direito com CTRL - Zoom-in/out\n");
	printf("PAGE_UP, PAGE_DOWN - Altera distância da camara \n");
	printf("ESC - Sair\n");
	listNos();
}

void putFoco() {
	const GLfloat white_amb[] = { 0.2, 0.2, 0.2, 1.0 };
	const GLfloat dif2[] = { 0.0, 0.0, 0.8, 1.0 };
	//FOCO
	GLfloat luz_dir[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float luzpos[] = { modelo.objecto.pos.x , modelo.objecto.pos.y , modelo.objecto.pos.z + 2, 1.0 };
	float luzdir[] = { 0.0, 0.0, -1.0 };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, blue_light);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_amb);

	glLightfv(GL_LIGHT1, GL_POSITION, luzpos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, luz_dir);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0); // qualquer valor entre 0.0 e 90.0 graus
}

void desenhaSolo() {
#define STEP 10
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	for (int i = -300; i<300; i += STEP)
		for (int j = -300; j<300; j += STEP) {
			glVertex2f(i, j);
			glVertex2f(i + STEP, j);
			glVertex2f(i + STEP, j + STEP);
			glVertex2f(i, j + STEP);
		}
	glEnd();
}

void CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[])
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

GLdouble VectorNormalize(GLdouble v[])
{
	int		i;
	GLdouble	length;

	if (fabs(v[1] - 0.000215956) < 0.0001)
		i = 1;

	length = 0;
	for (i = 0; i< 3; i++)
		length += v[i] * v[i];
	length = sqrt(length);
	if (length == 0)
		return 0;

	for (i = 0; i< 3; i++)
		v[i] /= length;

	return length;
}



void desenhaNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], tipo_material mat) {

	switch (mat) {
	case red_plastic:
		glColor3f(1, 0, 0);
		break;
	case azul:
		glColor3f(0, 0, 1);
		break;
	case emerald:
		glColor3f(0, 1, 0);
		break;
	default:
		glColor3f(1, 1, 0);
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslated(x, y, z);
	glScaled(0.4, 0.4, 0.4);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3dv(normal);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}


void desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, int orient) {
	zi = zi - 0.001;
	zf = zf - 0.001;
	GLdouble v1[3], v2[3], cross[3];
	GLdouble length;
	v1[0] = xf - xi;
	v1[1] = 0;
	v2[0] = 0;
	v2[1] = yf - yi;

	switch (orient) {
	case NORTE_SUL:
		v1[2] = 0;
		v2[2] = zf - zi;
		CrossProduct(v1, v2, cross);
		//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
		length = VectorNormalize(cross);
		//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);

		material(cinza);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textures.chao.TextureID);

		glBegin(GL_QUADS);
		glNormal3dv(cross);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(xi, yi, zi);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(xf, yi, zi);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(xf, yf, zf);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(xi, yf, zf);
		glEnd();		

		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);		

		break;
	
	}
}
void desenhaChaoRedondo(float largura, GLfloat x0, GLfloat y0, GLfloat z) {
	double x, y;
	material(cinza);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures.rotunda.TextureID);

	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	int n = 30;
	double alfa = 2 * M_PI / n;
	double ang = 0;
	glNormal3f(0, 0, 1);
	float coordx = 0.0, coordy = 0.0;

	for (int i = 0; i < n; i++) {
		 x = x0 + largura * cos(ang);
		 y = y0 + largura * sin(ang);

		 coordx = cos(ang)*0.5 + 0.5;
		 coordy = sin(ang)*0.5 + 0.5;

		glTexCoord2f(coordx, coordy);
		glVertex3f(x, y,z);
		ang += alfa;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);	
	
}

void desenhaNo(int no) {
	GLboolean norte, sul, este, oeste;
	GLfloat larguraNorte, larguraSul, larguraEste, larguraOeste;
	Arco arco = arcos[0];
	No *noi = &nos[no], *nof;
	norte = sul = este = oeste = GL_TRUE;
	//desenhaChao(nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, PLANO);
	desenhaChaoRedondo(noi->largura, noi->x, noi->y, noi->z);

}
void desenhaElemLigaInicial(Arco arco){
	No *noi, *nof;
	noi = &nos[arco.noi];
	nof = &nos[arco.nof];
	float si = K_LIGACAO * noi->largura;
	float xi = noi->x;
	float xf = nof->x;
	float yi = noi->y;
	float yf = nof->y;
	float zi = noi->z;
	float zf = nof->z;

	float orientacao_a = atan2f((yf - yi), (xf - xi));

	glPushMatrix();
	glTranslatef(xi, yi, zi);
	glRotatef(graus(orientacao_a),0,0,1);
	glTranslatef(si/2,0,0);


	desenhaChao(-si*0.5, -arco.largura*0.5, 0, si*0.5, arco.largura*0.5, 0, NORTE_SUL);
	glPopMatrix();

}

void desenhaElemLigaFinal(Arco arco) {
	No *noi, *nof;
	noi = &nos[arco.nof];
	nof = &nos[arco.noi];
	float si = K_LIGACAO * noi->largura;
	float xi = noi->x;
	float xf = nof->x;
	float yi = noi->y;
	float yf = nof->y;
	float zi = noi->z;
	float zf = nof->z;

	float orientacao_a = atan2f((yf - yi), (xf - xi));

	glPushMatrix();
	glTranslatef(xi, yi, zi);
	glRotatef(graus(orientacao_a), 0, 0, 1);
	glTranslatef(si / 2, 0, 0);

	desenhaChao(-si*0.5, -arco.largura*0.5, 0, si*0.5, arco.largura*0.5, 0, NORTE_SUL);
	glPopMatrix();

}

void desenhaArco(Arco arco) {
	No *noi, *nof;
	noi = &nos[arco.noi];
	nof = &nos[arco.nof];

	float si = K_LIGACAO * noi->largura;
	float sf = K_LIGACAO * nof->largura;

	float xi = noi->x;
	float xf = nof->x;
	float yi = noi->y;
	float yf = nof->y;
	float zi = noi->z;
	float zf = nof->z;
	float comp_p = sqrt(pow(xf - xi, 2) + pow(yf - yi, 2)) - si - sf;
	float desnivel_h = nof->z - noi->z;
	float comprimento_sif = sqrt(pow(comp_p, 2) + pow(desnivel_h, 2));
	float orientacao_a = atan2f((yf - yi), (xf - xi));
	float inclinacao_B = atan2f(desnivel_h, comp_p);

	glPushMatrix();
	glTranslatef(xi, yi, zi);
	glRotatef(graus(orientacao_a), 0, 0, 1);
	glTranslatef(si, 0, 0);
	glRotatef(graus(-inclinacao_B), 0, 1, 0);
	glTranslatef(comprimento_sif / 2.0, 0, 0);
	desenhaChao(-comprimento_sif*0.5, -arco.largura*0.5, 0, comprimento_sif*0.5, arco.largura*0.5, 0, NORTE_SUL);
	glPopMatrix();
}

void desenhaGrafo() {
	glPushMatrix();
	glTranslatef(0, 0, 0.05);
//	glScalef(5, 5, 5);
	//material(red_plastic);
	for (int i = 0; i<numNos; i++) {
		glPushMatrix();
		glTranslatef(nos[i].x, nos[i].y, nos[i].z + 0.25);
		glTranslatef(0, 0, 1.2);
		glPushMatrix();
		glScalef(SCALE_POI, SCALE_POI, SCALE_POI);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		mdlviewer_display(modelo.poi);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPopMatrix();
		glPopMatrix();
		desenhaNo(i);
	}
	//material(emerald);
	for (int i = 0; i < numArcos; i++) {
		desenhaArco(arcos[i]);
		desenhaElemLigaInicial(arcos[i]);
		desenhaElemLigaFinal(arcos[i]);
	}
	glPopMatrix();
}

void desenhaEixo() {
	gluCylinder(modelo.quad, 0.5, 0.5, 20, 16, 15);
	glPushMatrix();
	glTranslatef(0, 0, 20);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	gluDisk(modelo.quad, 0.5, 2, 16, 6);
	glPopMatrix();
	gluCylinder(modelo.quad, 2, 0, 5, 16, 15);
	glPopMatrix();
}

#define EIXO_X		1
#define EIXO_Y		2
#define EIXO_Z		3

void desenhaPlanoDrag(int eixo) {
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslated(estado.eixo[0], estado.eixo[1], estado.eixo[2]);
	switch (eixo) {
	case EIXO_Y:
		if (abs(estado.camera.dir_lat)<M_PI / 4)
			glRotatef(-90, 0, 0, 1);
		else
			glRotatef(90, 1, 0, 0);
		material(red_plastic);
		break;
	case EIXO_X:
		if (abs(estado.camera.dir_lat)>M_PI / 6)
			glRotatef(90, 1, 0, 0);
		material(azul);
		break;
	case EIXO_Z:
		if (abs(cos(estado.camera.dir_long))>0.5)
			glRotatef(90, 0, 0, 1);

		material(emerald);
		break;
	}
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-100, 0, -100);
	glVertex3f(100, 0, -100);
	glVertex3f(100, 0, 100);
	glVertex3f(-100, 0, 100);
	glEnd();
	glPopMatrix();
}

void desenhaEixos() {

	glPushMatrix();
	glTranslated(estado.eixo[0], estado.eixo[1], estado.eixo[2]);
	material(emerald);
	glPushName(EIXO_Z);
	desenhaEixo();
	glPopName();
	glPushName(EIXO_Y);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	material(red_plastic);
	desenhaEixo();
	glPopMatrix();
	glPopName();
	glPushName(EIXO_X);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	material(azul);
	desenhaEixo();
	glPopMatrix();
	glPopName();
	glPopMatrix();
}


void displayNavigateWindow(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	setCamera();
	putFoco();
	skybox->Render((float)abs(estado.camera.dir_long) * 6, (float)(estado.camera.dir_lat) * 6);

	glPushMatrix();
	glTranslatef(modelo.objecto.pos.x, modelo.objecto.pos.y, modelo.objecto.pos.z);
	glRotatef(graus(modelo.objecto.dir), 0, 0, 1);
	glRotatef(90, 0, 0, 1);
	glScalef(SCALE_HOMER, SCALE_HOMER, SCALE_HOMER);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	mdlviewer_display(modelo.personagem);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	material(slate);
	desenhaSolo();
	desenhaEixos();
	desenhaGrafo();
	if (estado.eixoTranslaccao) {
		// desenha plano de translacção
		cout << "Translate... " << estado.eixoTranslaccao << endl;
		desenhaPlanoDrag(estado.eixoTranslaccao);
	}

	//kms
	material(azul);
	OverlaysDesign ui = OverlaysDesign();
	ui.desenhaKm(modelo.km);


	//Tempo
	time_t now = time(0);
	int tempoDecorrido = now - modelo.tempo;
	int mins = tempoDecorrido / 60;
	int segs = tempoDecorrido - 60 * mins;
	ui.desenhaTempo(mins, segs);

	glFlush();

	glutSwapBuffers();
}

void setTopSubwindowCamera(objecto_t obj){
	


	Vertice eye;
	/*eye[0] = modelo.objecto.pos.x;
	eye[1] = modelo.objecto.pos.y-2;
	eye[2] = modelo.objecto.pos.z;*/
	eye[0] = estado.camera.center[0] + estado.camera.dist*cos(estado.camera.dir_long)*cos(estado.camera.dir_lat);
	eye[1] = estado.camera.center[1] + estado.camera.dist*sin(estado.camera.dir_long)*cos(estado.camera.dir_lat);
	eye[2] = 80;

	if (estado.light) {
		gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
		putLights((GLfloat*)white_light);
	}
	else {
		putLights((GLfloat*)white_light);
		gluLookAt(eye[0], eye[1], eye[2], estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], 0, 0, 1);
	}

}
void desenhaAngVisao()
{
	GLfloat ratio;
	ratio = (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glPushMatrix();
	glTranslatef(estado.camera.center[0], estado.camera.center[1], OBJECTO_ALTURA);
	glColor4f(0, 0, 1, 0.2);
	glRotatef(graus(estado.camera.dir_long), 0, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(5 * cos(rad(estado.camera.fov*ratio*0.5)), 0, -5 * sin(rad(estado.camera.fov*ratio*0.5)));
	glVertex3f(5 * cos(rad(estado.camera.fov*ratio*0.5)), 0, 5 * sin(rad(estado.camera.fov*ratio*0.5)));
	glEnd();
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}



void displayTopWindow() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	setTopSubwindowCamera(modelo.objecto);

	glPushMatrix();
	glTranslatef(modelo.objecto.pos.x, modelo.objecto.pos.y, modelo.objecto.pos.z);
	glRotatef(graus(modelo.objecto.dir), 0, 0, 1);
	glRotatef(90, 0, 0, 1);
	int scale_top = 10;
	glScalef(SCALE_HOMER*scale_top, SCALE_HOMER*scale_top, SCALE_HOMER*scale_top);
	glDisable(GL_LIGHTING);
	mdlviewer_display(modelo.personagem);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	material(slate);
	desenhaSolo();
	desenhaGrafo();
	desenhaAngVisao();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'h':
	case 'H':
		imprime_ajuda();
		break;
	case 'l':
	case 'L':
		if (estado.lightViewer)
			estado.lightViewer = 0;
		else
			estado.lightViewer = 1;
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.lightViewer);
		redisplayAll();
		break;
	case 'k':
	case 'K':
		estado.light = !estado.light;
		redisplayAll();
		break;
	case 'w':
	case 'W':
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		redisplayAll();
		break;
	case 'p':
	case 'P':
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		redisplayAll();
		break;
	case 's':
	case 'S':
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		redisplayAll();
		break;
	case 'c':
	case 'C':
		if (glIsEnabled(GL_CULL_FACE))
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);
		redisplayAll();
		break;
	case 'n':
	case 'N':
		estado.apresentaNormais = !estado.apresentaNormais;
		redisplayAll();
		break;
	case 'i':
	case 'I':
		initEstado();
		initModelo();
		redisplayAll();
		break;

	case 'f':
	case 'F':
		estado.isFP = !estado.isFP;
		break;
	}
}

void Special(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_F1:
		//gravaGrafo();

		break;
	case GLUT_KEY_F2:
		leGrafo();
		redisplayAll();
		break;

	case GLUT_KEY_F3:

		redisplayAll();
		break;

	/*case GLUT_KEY_PAGE_UP:
		if(estado.camera.dist>2)
			estado.camera.dist--;
		break;

	case GLUT_KEY_PAGE_DOWN:
			estado.camera.dist++;
		break;*/

	case GLUT_KEY_F6:
		numNos = numArcos = 0;
		addNo(criaNo(0, 10, 0));  // 0
		addNo(criaNo(0, 5, 0));  // 1
		addNo(criaNo(-5, 5, 0));  // 2
		addNo(criaNo(5, 5, 0));  // 3
		addNo(criaNo(-5, 0, 0));  // 4
		addNo(criaNo(5, 0, 0));  // 5
		addNo(criaNo(-5, -5, 0));  // 6
		addArco(criaArco(0, 1, 1, 1));  // 0 - 1
		addArco(criaArco(1, 2, 1, 1));  // 1 - 2
		addArco(criaArco(1, 3, 1, 1));  // 1 - 3
		addArco(criaArco(2, 4, 1, 1));  // 2 - 4
		addArco(criaArco(3, 5, 1, 1));  // 3 - 5
		addArco(criaArco(4, 5, 1, 1));  // 4 - 5
		addArco(criaArco(4, 6, 1, 1));  // 4 - 6
		redisplayAll();
		break;

	case GLUT_KEY_UP: estado.teclas.up = GL_TRUE;
		break;
	case GLUT_KEY_DOWN: estado.teclas.down = GL_TRUE;
		break;
	case GLUT_KEY_LEFT: estado.teclas.left = GL_TRUE;
		break;
	case GLUT_KEY_RIGHT: estado.teclas.right = GL_TRUE;
		break;
	}
}

// Callback para interaccao via teclas especiais (largar na tecla)
void SpecialKeyUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: estado.teclas.up = GL_FALSE;
		break;
	case GLUT_KEY_DOWN: estado.teclas.down = GL_FALSE;
		break;
	case GLUT_KEY_LEFT: estado.teclas.left = GL_FALSE;
		break;
	case GLUT_KEY_RIGHT: estado.teclas.right = GL_FALSE;
		break;
	}
}


void setProjection(int x, int y, GLboolean picking) {
	glLoadIdentity();
	if (picking) { // se está no modo picking, lê viewport e define zona de picking
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 4, 4, vport); // Inverte o y do rato para corresponder à jana
	}

	gluPerspective(estado.camera.fov, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT), 0.6, 500);

}

void myReshapeNavigateWindow(int width, int height) {
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(estado.camera.fov, (GLfloat)width / height, 0.1, 50);
	glMatrixMode(GL_MODELVIEW);
}

void motionRotate(int x, int y) {
#define DRAG_SCALE	0.01
	double lim = M_PI / 2 - 0.1;
	estado.camera.dir_long += (estado.xMouse - x)*DRAG_SCALE;
	estado.camera.dir_lat -= (estado.yMouse - y)*DRAG_SCALE*0.5;
	if (estado.camera.dir_lat>lim)
		estado.camera.dir_lat = lim;
	else
		if (estado.camera.dir_lat<-lim)
			estado.camera.dir_lat = -lim;
	estado.xMouse = x;
	estado.yMouse = y;
	redisplayAll();
}

void motionZoom(int x, int y) {
#define ZOOM_SCALE	0.5
	estado.camera.dist -= (estado.yMouse - y)*ZOOM_SCALE;
	if (estado.camera.dist<5)
		estado.camera.dist = 5;
	else
		if (estado.camera.dist>200)
			estado.camera.dist = 200;
	estado.yMouse = y;
	redisplayAll();
}

void motionDrag(int x, int y) {
	GLuint buffer[100];
	GLint vp[4];
	GLdouble proj[16], mv[16];
	int n;
	GLdouble newx, newy, newz;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
	glLoadIdentity();
	setProjection(x, y, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	desenhaPlanoDrag(estado.eixoTranslaccao);

	n = glRenderMode(GL_RENDER);
	if (n > 0) {
		glGetIntegerv(GL_VIEWPORT, vp);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);
		gluUnProject(x, glutGet(GLUT_WINDOW_HEIGHT) - y, (double)buffer[2] / UINT_MAX, mv, proj, vp, &newx, &newy, &newz);
		printf("Novo x:%lf, y:%lf, z:%lf\n\n", newx, newy, newz);
		switch (estado.eixoTranslaccao) {
		case EIXO_X:
			estado.eixo[0] = newx;
			//estado.eixo[1]=newy;
			break;
		case EIXO_Y:
			estado.eixo[1] = newy;
			//estado.eixo[2]=newz;
			break;
		case EIXO_Z:
			//estado.eixo[0]=newx;
			estado.eixo[2] = newz;
			break;
		}
		redisplayAll();
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	redisplayAll();
}

int picking(int x, int y) {
	int i, n, objid = 0;
	double zmin = 10.0;
	GLuint buffer[100], *ptr;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
	glLoadIdentity();
	setProjection(x, y, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setCamera();
	desenhaEixos();

	n = glRenderMode(GL_RENDER);
	if (n > 0)
	{
		ptr = buffer;
		for (i = 0; i < n; i++)
		{
			if (zmin >(double) ptr[1] / UINT_MAX) {
				zmin = (double)ptr[1] / UINT_MAX;
				objid = ptr[3];
			}
			ptr += 3 + ptr[0]; // ptr[0] contem o número de nomes (normalmente 1); 3 corresponde a numnomes, zmin e zmax
		}
	}


	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	return objid;
}
void mouse(int btn, int state, int x, int y) {
	switch (btn) {
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			estado.xMouse = x;
			estado.yMouse = y;
			if (glutGetModifiers() & GLUT_ACTIVE_CTRL)
				glutMotionFunc(motionZoom);
			else
				glutMotionFunc(motionRotate);
			cout << "Left down\n";
		}
		else {
			glutMotionFunc(NULL);
			cout << "Left up\n";
		}
		break;
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			estado.eixoTranslaccao = picking(x, y);
			if (estado.eixoTranslaccao)
				glutMotionFunc(motionDrag);
			cout << "Right down - objecto:" << estado.eixoTranslaccao << endl;
		}
		else {
			if (estado.eixoTranslaccao != 0) {
				estado.camera.center[0] = estado.eixo[0];
				estado.camera.center[1] = estado.eixo[1];
				estado.camera.center[2] = estado.eixo[2];
				glutMotionFunc(NULL);
				estado.eixoTranslaccao = 0;
				redisplayAll();
			}
			cout << "Right up\n";
		}
		break;
	}
}

int main(int argc, char **argv){
	ConsolaMenu consola = ConsolaMenu();
	consola.consolaMain();

	glutInit(&argc, argv);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800 + GAP * 3, 400 + GAP * 2);
	/* need both double buffering and z buffer */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	if ((estado.mainWindow = glutCreateWindow("LAPR5")) == GL_FALSE)
		exit(1);

	myInit();
	glutReshapeFunc(myReshapeMainWindow);
	glutDisplayFunc(displayMainWindow);
	glutTimerFunc(estado.timer, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutSpecialUpFunc(SpecialKeyUp);
	
	//subwindow
	estado.topSubwindow = glutCreateSubWindow(estado.mainWindow, GAP, GAP, 400, 400);
	myInit();
	glutReshapeFunc(myReshapeTopWindow);
	glutDisplayFunc(displayTopWindow);
	glutTimerFunc(estado.timer, timer, 0);
	mdlviewer_init("homer.mdl", modelo.personagem);
	modelo.personagem.SetSequence(3);
	mdlviewer_init("Yoda.mdl", modelo.poi);

	//subwindow
	estado.navigateSubwindow = glutCreateSubWindow(estado.mainWindow, 400+GAP, GAP, 400, 800);
	myInit();	
	glutReshapeFunc(myReshapeNavigateWindow);
	glutDisplayFunc(displayNavigateWindow);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(mouse);
	glutSpecialUpFunc(SpecialKeyUp);
	glutTimerFunc(estado.timer, timer, 0);
	mdlviewer_init("homer.mdl", modelo.personagem);
	modelo.personagem.SetSequence(3);
	mdlviewer_init("Yoda.mdl", modelo.poi);
	imprime_ajuda();

	skybox = new SKYBOX();
	if (skybox->Initialize()){
		// Início da aplicação
		glutMainLoop();

		// Destruição da skybox
		skybox->Finalize();
		delete skybox;

		return 0;
	}


	return 1;
}