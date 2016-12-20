#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <GL/glut.h>
#include <GL/glaux.h>

#include <AL/alut.h>

#include "mathlib.h"
#include "studio.h"
#include "mdlviewer.h"

#include "grafos.h"
#include "som.h"
#include "Model_3DS.h"
#include "Monumentos.h"
#include "DesenhaLabirinto.h"

#include "OverlaysDesign.h"
#include "Estado.h"


#pragma comment (lib, "glaux.lib")    /* link with Win32 GLAUX lib */
#pragma comment (lib, "openAL32.lib")
#pragma comment (lib, "alut.lib")
#pragma comment (user, "Compiled on " __DATE__ " at " __TIME__)


extern "C" int read_JPEG_file(char *, char **, int *, int *, int *);

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define RAD(x)          (M_PI*(x)/180)
#define GRAUS(x)        (180*(x)/M_PI)

#define	GAP					25

#define MAZE_HEIGHT			18
#define MAZE_WIDTH			18

#define	OBJECTO_ALTURA		  0.4
#define OBJECTO_VELOCIDADE	0.5
#define OBJECTO_ROTACAO		  5
#define OBJECTO_RAIO		    0.12
#define SCALE_HOMER         0.005
#define EYE_ROTACAO			    1

#define NOME_TEXTURA_CUBOS        "Textura.bmp"
#define NOME_TEXTURA_CHAO         "Chao.jpg"
#define NUM_TEXTURAS              2
#define ID_TEXTURA_CUBOS          0
#define ID_TEXTURA_CHAO           1

#define	CHAO_DIMENSAO		          10

#define NUM_JANELAS               2
#define JANELA_TOP                0
#define JANELA_NAVIGATE           1

typedef struct teclas_t {
	GLboolean   up, down, left, right;
}teclas_t;

typedef struct pos_t {
	GLfloat    x, y, z;
}pos_t;

typedef struct objecto_t {
	pos_t    pos;
	GLfloat  dir;
	GLfloat  vel;
}objecto_t;

typedef struct camera_t {
	pos_t    eye;
	GLfloat  dir_long;  // longitude olhar (esq-dir)
	GLfloat  dir_lat;   // latitude olhar	(cima-baixo)
	GLfloat  fov;
}camera_t;

typedef struct ESTADO {
	camera_t      camera;
	GLint         timer;
	GLint         mainWindow, topSubwindow, navigateSubwindow;
	teclas_t      teclas;
	GLboolean     localViewer;
	GLuint        vista[NUM_JANELAS];
	ALuint        buffer[9], source[3];
}ESTADO;

typedef struct MODELO {
	GLuint        texID[NUM_JANELAS][NUM_TEXTURAS];
	GLuint        labirinto[NUM_JANELAS];
	GLuint        chao[NUM_JANELAS];
	objecto_t	    objecto;
	GLuint        xMouse;
	GLuint        yMouse;
	StudioModel   homer[NUM_JANELAS];   // Modelo Homer
	GLboolean     andar;
	GLuint        prev;
	float         km;
	int		  tempo;
}MODELO;

/////////////////////////////////////
//variaveis globais

ESTADO estado;
Estado estadoGrafo = Estado();
MODELO modelo;
Model_3DS clerigos, casaMusica;


char mazedata[MAZE_HEIGHT][MAZE_WIDTH + 1] = {
	"                  ",
	" ******* ******** ",
	" *       *      * ",
	" * * *** * *    * ",
	" * **  * ** * * * ",
	" *     *      * * ",
	" *          *** * ",
	" *           *  * ",
	" *     * *** **** ",
	" * *   *   *    * ",
	" *   ****  *    * ",
	" ********  **** * ",
	" *            * * ",
	" *     *      * * ",
	" ** ** *    *** * ",
	" *   *      *   * ",
	" *******  **** ** ",
	"                  "
};


////////////////////////////////////
/// Iluminação e materiais

void setLight(){
	GLfloat light_pos[4] = { -5.0, 20.0, -8.0, 0.0 };
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// laar iluminação
	glEnable(GL_LIGHTING);
	// ligar e definir fonte de light 0
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.localViewer);
}

void setMaterial()
{
	GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_shininess = 104;
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// definir os outros parâmetros estaticamente
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

///////////////////////////////////
//// Redisplays

void redisplayTopSubwindow(int width, int height){
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


void reshapeNavigateSubwindow(int width, int height){
	// glViewport(botom, left, width, height)
	// define parte da janela a ser utilizada pelo OpenGL
	glViewport(0, 0, (GLint)width, (GLint)height);
	// Matriz Projeccao
	// Matriz onde se define como o mundo e apresentado na janela
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(estado.camera.fov, (GLfloat)width / height, 0.1, 50);
	// Matriz Modelview
	// Matriz onde são realizadas as tranformacoes dos modelos desenhados
	glMatrixMode(GL_MODELVIEW);
}

void reshapeMainWindow(int width, int height){
	GLint w, h;
	w = (width - GAP * 3)*.5;
	h = (height - GAP * 2);
	glutSetWindow(estado.topSubwindow);
	glutPositionWindow(GAP, GAP);
	glutReshapeWindow(w, h);
	glutSetWindow(estado.navigateSubwindow);
	glutPositionWindow(GAP + w + GAP, GAP);
	glutReshapeWindow(w, h);
}

/////////////////////////////////////
//Modelo


GLboolean detectaColisao(GLfloat nx, GLfloat nz)
{
	/*GLuint i = (nx + MAZE_HEIGHT*0.5 + 0.5), j = (int)(nz + MAZE_WIDTH*0.5 + 0.5);
	if (mazedata[i][j] == '*')
	{
		if (modelo.homer[JANELA_NAVIGATE].GetSequence() != 20)
		{
			modelo.homer[JANELA_TOP].SetSequence(20);
			modelo.homer[JANELA_NAVIGATE].SetSequence(20);
		}
		alSourceStop(estado.source[1]);
		alSourcei(estado.source[1], AL_BUFFER, estado.buffer[rand() % 3 + 6]);
		alSourcePlay(estado.source[1]);
		return(GL_TRUE);
	}*/
	return(GL_FALSE);
}

void strokeCenterString(char *str, double x, double y, double z, double s)
{
	int i, n;

	n = strlen(str);
	glPushMatrix();
	glTranslated(x - glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char*)str)*0.5*s, y - 119.05*0.5*s, z);
	glScaled(s, s, s);
	for (i = 0; i<n; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)str[i]);
	glPopMatrix();

}

void desenhaPoligono(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat  d[], GLfloat normal[], GLfloat tx, GLfloat ty)
{
	glBegin(GL_POLYGON);
	glNormal3fv(normal);
	glTexCoord2f(tx + 0, ty + 0);
	glVertex3fv(a);
	glTexCoord2f(tx + 0, ty + 0.25);
	glVertex3fv(b);
	glTexCoord2f(tx + 0.25, ty + 0.25);
	glVertex3fv(c);
	glTexCoord2f(tx + 0.25, ty + 0);
	glVertex3fv(d);
	glEnd();
}


void desenhaCubo(int tipo, GLuint texID)
{
	GLfloat vertices[][3] = { { -0.5,-0.5,-0.5 },
	{ 0.5,-0.5,-0.5 },
	{ 0.5,0.5,-0.5 },
	{ -0.5,0.5,-0.5 },
	{ -0.5,-0.5,0.5 },
	{ 0.5,-0.5,0.5 },
	{ 0.5,0.5,0.5 },
	{ -0.5,0.5,0.5 } };
	GLfloat normais[][3] = { { 0,0,-1 },
	{ 0,1,0 },
	{ -1,0,0 },
	{ 1,0,0 },
	{ 0,0,1 },
	{ 0,-1,0 } };


	GLfloat tx, ty;

	switch (tipo)
	{
	case 0: tx = 0, ty = 0;
		break;
	case 1: tx = 0, ty = 0.25;
		break;
	case 2: tx = 0, ty = 0.5;
		break;
	case 3: tx = 0, ty = 0.75;
		break;
	case 4: tx = 0.25, ty = 0;
		break;

	default:
		tx = 0.75, ty = 0.75;
	}
	glBindTexture(GL_TEXTURE_2D, texID);

	desenhaPoligono(vertices[1], vertices[0], vertices[3], vertices[2], normais[0], tx, ty);
	desenhaPoligono(vertices[2], vertices[3], vertices[7], vertices[6], normais[1], tx, ty);
	desenhaPoligono(vertices[3], vertices[0], vertices[4], vertices[7], normais[2], tx, ty);
	desenhaPoligono(vertices[6], vertices[5], vertices[1], vertices[2], normais[3], tx, ty);
	desenhaPoligono(vertices[4], vertices[5], vertices[6], vertices[7], normais[4], tx, ty);
	desenhaPoligono(vertices[5], vertices[4], vertices[0], vertices[1], normais[5], tx, ty);
	glBindTexture(GL_TEXTURE_2D, NULL);
}


void desenhaBussola(int width, int height)
{

	// Altera viewport e projecção

	glViewport(width - 60, 0, 60, 60);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluOrtho2D(-30, 30, -30, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
	glRotatef(GRAUS(-estado.camera.dir_long + estado.camera.dir_lat) - 90, 0, 0, 1);

	glBegin(GL_TRIANGLES);
	glColor4f(0, 0, 0, 0.2);
	glVertex2f(0, 15);
	glVertex2f(-6, 0);
	glVertex2f(6, 0);
	glColor4f(1, 1, 1, 0.2);
	glVertex2f(6, 0);
	glVertex2f(-6, 0);
	glVertex2f(0, -15);
	glEnd();

	glLineWidth(1.0);
	glColor3f(1, 0.4, 0.4);
	strokeCenterString("N", 0, 20, 0, 0.1);
	strokeCenterString("S", 0, -20, 0, 0.1);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);


	//repõe projecção chamando redisplay
	reshapeNavigateSubwindow(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

}

void desenhaModeloDir(objecto_t obj, int width, int height)
{
	// Altera viewport e projecção
	glViewport(width - 60, 0, 60, 60);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glRotatef(GRAUS(obj.dir), 0, 0, 1);

	glBegin(GL_QUADS);
	glColor4f(1, 0, 0, 0.5);
	glVertex2f(5, 2.5);
	glVertex2f(-10, 2.5);
	glVertex2f(-10, -2.5);
	glVertex2f(5, -2.5);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(10, 0);
	glVertex2f(5, 5);
	glVertex2f(5, -5);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	//repõe projecção chamando redisplay
	redisplayTopSubwindow(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void desenhaAngVisao(camera_t *cam)
{
	GLfloat ratio;
	ratio = (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glPushMatrix();
	glTranslatef(cam->eye.x, OBJECTO_ALTURA, cam->eye.z);
	glColor4f(0, 0, 1, 0.2);
	glRotatef(GRAUS(cam->dir_long), 0, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(5 * cos(RAD(cam->fov*ratio*0.5)), 0, -5 * sin(RAD(cam->fov*ratio*0.5)));
	glVertex3f(5 * cos(RAD(cam->fov*ratio*0.5)), 0, 5 * sin(RAD(cam->fov*ratio*0.5)));
	glEnd();
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void desenhaGrafo() {
	DesenhaLabirinto dLabirinto = DesenhaLabirinto();
	//clerigos.Draw();

	glPushMatrix();
	dLabirinto.desenhaLabirinto(numNos, numArcos, GL_TRUE);
	glPopMatrix();
}

void desenhaModelo()
{
	glColor3f(0, 1, 0);
	glutSolidCube(OBJECTO_ALTURA);
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0, OBJECTO_ALTURA*0.75, 0);
	glRotatef(GRAUS(estado.camera.dir_long - modelo.objecto.dir), 0, 1, 0);
	glutSolidCube(OBJECTO_ALTURA*0.5);
	glPopMatrix();

}

void desenhaLabirinto(GLuint texID)
{
	int i, j;
	glColor3f(0.8f, 0.8f, 0.8f);
	glPushMatrix();
	glTranslatef(-MAZE_HEIGHT*0.5, 0.5, -MAZE_WIDTH*0.5);
	for (i = 0; i<MAZE_HEIGHT; i++)
		for (j = 0; j<MAZE_WIDTH; j++)
			if (mazedata[i][j] == '*')
			{
				glPushMatrix();
				glTranslatef(i, 0, j);
				desenhaCubo((i + j) % 6, texID);
				glPopMatrix();
			}
	glPopMatrix();
}

void desenhaChao(GLfloat dimensao, GLuint texID)
{
	GLfloat i, j;
#define STEP    1
	glBindTexture(GL_TEXTURE_2D, texID);

	glColor3f(0.5f, 0.5f, 0.5f);
	for (i = -dimensao; i <= dimensao; i += STEP)
		for (j = -dimensao; j <= dimensao; j += STEP)
		{
			glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glTexCoord2f(1, 1);
			glVertex3f(i + STEP, 0, j + STEP);
			glTexCoord2f(0, 1);
			glVertex3f(i, 0, j + STEP);
			glTexCoord2f(0, 0);
			glVertex3f(i, 0, j);
			glTexCoord2f(1, 0);
			glVertex3f(i + STEP, 0, j);
			glEnd();
		}
	glBindTexture(GL_TEXTURE_2D, NULL);
}

/////////////////////////////////////
//navigateSubwindow
void motionNavigateSubwindow(int x, int y)
{
	int dif;
	dif = y - modelo.yMouse;
	if (dif>0) {//olhar para baixo
		estado.camera.dir_lat -= dif*RAD(EYE_ROTACAO);
		if (estado.camera.dir_lat<-RAD(45))
			estado.camera.dir_lat = -RAD(45);
	}

	if (dif<0) {//olhar para cima
		estado.camera.dir_lat += abs(dif)*RAD(EYE_ROTACAO);
		if (estado.camera.dir_lat>RAD(45))
			estado.camera.dir_lat = RAD(45);
	}

	dif = x - modelo.xMouse;

	if (dif>0) { //olhar para a direita
		estado.camera.dir_long -= dif*RAD(EYE_ROTACAO);
		/*
		if(estado.camera.dir_long<modelo.objecto.dir-RAD(45))
		estado.camera.dir_long=modelo.objecto.dir-RAD(45);
		*/
	}
	if (dif<0) {//olhar para a esquerda
		estado.camera.dir_long += abs(dif)*RAD(EYE_ROTACAO);
		/*
		if(estado.camera.dir_long>modelo.objecto.dir+RAD(45))
		estado.camera.dir_long=modelo.objecto.dir+RAD(45);
		*/

	}
	modelo.xMouse = x;
	modelo.yMouse = y;

}


void mouseNavigateSubwindow(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			modelo.xMouse = x;
			modelo.yMouse = y;
			glutMotionFunc(motionNavigateSubwindow);
		}
		else
			glutMotionFunc(NULL);
	}
}

void setNavigateSubwindowCamera(camera_t *cam, objecto_t obj)
{

	pos_t center;

	if (estado.vista[JANELA_NAVIGATE])
	{
		cam->eye.x = obj.pos.x;
		cam->eye.y = obj.pos.y + .2;
		cam->eye.z = obj.pos.z;
		center.x = obj.pos.x + cos(cam->dir_long)*cos(cam->dir_lat);
		center.z = obj.pos.z + sin(-cam->dir_long)*cos(cam->dir_lat);
		center.y = cam->eye.y + sin(cam->dir_lat);
	}
	else
	{
		center.x = obj.pos.x;
		center.y = obj.pos.y + 0.5;
		center.z = obj.pos.z;

		cam->eye.x = center.x - cos(cam->dir_long);
		cam->eye.z = center.z - sin(-cam->dir_long);
		cam->eye.y = center.y + 0.5;

	}
	gluLookAt(cam->eye.x, cam->eye.y, cam->eye.z, center.x, center.y, center.z, 0, 1, 0);
}



void displayNavigateSubwindow()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	setNavigateSubwindowCamera(&estado.camera, modelo.objecto);
	//setLight();

	//glCallList(modelo.labirinto[JANELA_NAVIGATE]);
	glCallList(modelo.chao[JANELA_NAVIGATE]);


	if (!estado.vista[JANELA_NAVIGATE])
	{
		glPushMatrix();
		glTranslatef(modelo.objecto.pos.x, modelo.objecto.pos.y, modelo.objecto.pos.z);
		glRotatef(GRAUS(modelo.objecto.dir), 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
		glScalef(SCALE_HOMER, SCALE_HOMER, SCALE_HOMER);
		mdlviewer_display(modelo.homer[JANELA_NAVIGATE]);
		glPopMatrix();

		Monumentos monumentos = Monumentos();
		//monumentos.desenhaCasaMusica(casaMusica);
		monumentos.desenhaClerigos(clerigos);

		DesenhaLabirinto dLabirinto = DesenhaLabirinto();
		//material(slate);		
		//dLabirinto.desenhaEixos(estado.getEixo(0), estado.getEixo(1), estado.getEixo(2), modelo.quad);
		dLabirinto.desenhaLabirinto(numNos, numArcos, GL_TRUE);

	}

	desenhaBussola(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	OverlaysDesign ui = OverlaysDesign();
	ui.desenhaKm(modelo.km);

	time_t now = time(0);

	int tempoDecorrido = now - modelo.tempo;

	ui.desenhaTempo(tempoDecorrido);

	

	glutSwapBuffers();
}

/////////////////////////////////////
//topSubwindow
void setTopSubwindowCamera(camera_t *cam, objecto_t obj)
{
	cam->eye.x = obj.pos.x;
	cam->eye.z = obj.pos.z;
	if (estado.vista[JANELA_TOP])
		gluLookAt(obj.pos.x, CHAO_DIMENSAO*.2, obj.pos.z, obj.pos.x, obj.pos.y, obj.pos.z, 0, 0, -1);
	else
		gluLookAt(obj.pos.x, CHAO_DIMENSAO * 2, obj.pos.z, obj.pos.x, obj.pos.y, obj.pos.z, 0, 0, -1);
}

void displayTopSubwindow()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	setTopSubwindowCamera(&estado.camera, modelo.objecto);
	setLight();

	//glCallList(modelo.labirinto[JANELA_TOP]);
	glCallList(modelo.chao[JANELA_TOP]);
	desenhaGrafo();

	glPushMatrix();
	glTranslatef(modelo.objecto.pos.x, modelo.objecto.pos.y, modelo.objecto.pos.z);
	glRotatef(GRAUS(modelo.objecto.dir), 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(SCALE_HOMER, SCALE_HOMER, SCALE_HOMER);
	mdlviewer_display(modelo.homer[JANELA_TOP]);
	glPopMatrix();

	desenhaAngVisao(&estado.camera);
	desenhaModeloDir(modelo.objecto, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glFlush();
	glutSwapBuffers();
}


/////////////////////////////////////
//mainWindow

void redisplayAll(void)
{
	glutSetWindow(estado.mainWindow);
	glutPostRedisplay();
	glutSetWindow(estado.topSubwindow);
	glutPostRedisplay();
	glutSetWindow(estado.navigateSubwindow);
	glutPostRedisplay();

}

void displayMainWindow()
{
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();

}

void Timer(int value)
{


	GLfloat nx, nz;
	GLboolean andar = GL_FALSE;

	ALint state;
	alGetSourcei(estado.source[0], AL_SOURCE_STATE, &state);
	if (state == AL_STOPPED)
	{
		alSourcef(estado.source[0], AL_GAIN, 0.5f);
		alSourcei(estado.source[0], AL_BUFFER, estado.buffer[1]);
		alSourcePlay(estado.source[0]);
	}

	GLuint curr = GetTickCount();
	float velocidade = modelo.objecto.vel*(curr - modelo.prev)*0.001;

	if (modelo.homer[JANELA_NAVIGATE].GetSequence() != 20)
	{
		glutTimerFunc(estado.timer, Timer, 0);
	}
	else if (value<4500)
	{
		glutTimerFunc(estado.timer, Timer, value + curr - modelo.prev);
		redisplayAll();
		return;
	}
	else
	{
		modelo.homer[JANELA_NAVIGATE].SetSequence(0);
		modelo.homer[JANELA_TOP].SetSequence(0);
		glutTimerFunc(estado.timer, Timer, 0);
	}
	modelo.prev = curr;

	

	if (estado.teclas.up) {
		nx = modelo.objecto.pos.x + cos(modelo.objecto.dir)*velocidade;
		nz = modelo.objecto.pos.z + sin(-modelo.objecto.dir)*velocidade;
		if (!detectaColisao(nx + cos(modelo.objecto.dir)*OBJECTO_RAIO, nz + sin(-modelo.objecto.dir)*OBJECTO_RAIO) &&
			!detectaColisao(nx + cos(modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO, nz + sin(-modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO) &&
			!detectaColisao(nx + cos(modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO, nz + sin(-modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO)) {
			
		
			modelo.objecto.pos.x = nx;
			modelo.objecto.pos.z = nz;

			modelo.km += 0.5;
			
		}
		andar = GL_TRUE;

	}
	if (estado.teclas.down) {
		nx = modelo.objecto.pos.x - cos(modelo.objecto.dir)*velocidade;
		nz = modelo.objecto.pos.z - sin(-modelo.objecto.dir)*velocidade;
		if (!detectaColisao(nx, nz) &&
			!detectaColisao(nx - cos(modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO, nz - sin(-modelo.objecto.dir + M_PI / 4)*OBJECTO_RAIO) &&
			!detectaColisao(nx - cos(modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO, nz - sin(-modelo.objecto.dir - M_PI / 4)*OBJECTO_RAIO)) {
			modelo.objecto.pos.x = nx;
			modelo.objecto.pos.z = nz;

			modelo.km += 0.5;
		}
		andar = GL_TRUE;
	}
	if (estado.teclas.left) {
		modelo.objecto.dir += RAD(OBJECTO_ROTACAO);
		estado.camera.dir_long += RAD(OBJECTO_ROTACAO);
	}
	if (estado.teclas.right) {
		modelo.objecto.dir -= RAD(OBJECTO_ROTACAO);
		estado.camera.dir_long -= RAD(OBJECTO_ROTACAO);
	}

	if (modelo.homer[JANELA_NAVIGATE].GetSequence() != 20)
	{
		ALint state;
		alGetSourcei(estado.source[1], AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING && rand() % 200 == 0)
		{
			if (andar)
				alSourcei(estado.source[1], AL_BUFFER, estado.buffer[5]);
			else
				alSourcei(estado.source[1], AL_BUFFER, estado.buffer[2]);
			alSourcePlay(estado.source[1]);
		}
		if (andar && modelo.homer[JANELA_NAVIGATE].GetSequence() != 3)
		{
			modelo.homer[JANELA_NAVIGATE].SetSequence(3);
			modelo.homer[JANELA_TOP].SetSequence(3);
		}
		else
			if (!andar && modelo.homer[JANELA_NAVIGATE].GetSequence() != 0)
			{
				modelo.homer[JANELA_NAVIGATE].SetSequence(0);
				modelo.homer[JANELA_TOP].SetSequence(0);
			}
	}


	redisplayAll();

}


void imprime_ajuda(void)
{
	printf("\n\nDesenho de um quadrado\n");
	printf("h,H - Ajuda \n");
	printf("******* Diversos ******* \n");
	printf("l,L - Alterna o calculo luz entre Z e eye (GL_LIGHT_MODEL_LOCAL_VIEWER)\n");
	printf("w,W - Wireframe \n");
	printf("s,S - Fill \n");
	printf("******* Movimento ******* \n");
	printf("up  - Acelera \n");
	printf("down- Trava \n");
	printf("left- Vira rodas para a direita\n");
	printf("righ- Vira rodas para a esquerda\n");
	printf("******* Camara ******* \n");
	printf("F1 - Alterna camara da janela da Esquerda \n");
	printf("F2 - Alterna camara da janela da Direita \n");
	printf("PAGE_UP, PAGE_DOWN - Altera abertura da camara \n");
	printf("botao esquerdo + movimento na Janela da Direita altera o olhar \n");
	printf("ESC - Sair\n");
}


void Key(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(1);
		break;
	case 'h':
	case 'H':
		imprime_ajuda();
		break;
	case 'l':
	case 'L':
		estado.localViewer = !estado.localViewer;
		break;
	case 'w':
	case 'W':
		glutSetWindow(estado.navigateSubwindow);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_2D);
		glutSetWindow(estado.topSubwindow);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_2D);
		break;
	case 's':
	case 'S':
		glutSetWindow(estado.navigateSubwindow);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
		glutSetWindow(estado.topSubwindow);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
		break;
	}

}
void SpecialKey(int key, int x, int y)
{
	ALint state;
	switch (key) {
	case GLUT_KEY_UP: estado.teclas.up = GL_TRUE;
		break;
	case GLUT_KEY_DOWN: estado.teclas.down = GL_TRUE;
		break;
	case GLUT_KEY_LEFT: estado.teclas.left = GL_TRUE;
		break;
	case GLUT_KEY_RIGHT: estado.teclas.right = GL_TRUE;
		break;
	case GLUT_KEY_F1:
		estado.vista[JANELA_TOP] = !estado.vista[JANELA_TOP];
		alGetSourcei(estado.source[2], AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alSourcei(estado.source[2], AL_BUFFER, estado.buffer[3]);
			alSourcePlay(estado.source[2]);
		}
		break;
	case GLUT_KEY_F2:
		estado.vista[JANELA_NAVIGATE] = !estado.vista[JANELA_NAVIGATE];
		alGetSourcei(estado.source[2], AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)
		{
			alSourcei(estado.source[2], AL_BUFFER, estado.buffer[4]);
			alSourcePlay(estado.source[2]);
		}
		break;
	case GLUT_KEY_PAGE_UP:
		if (estado.camera.fov>20)
		{
			estado.camera.fov--;
			glutSetWindow(estado.navigateSubwindow);
			reshapeNavigateSubwindow(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			redisplayAll();
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (estado.camera.fov<130)
		{
			estado.camera.fov++;
			glutSetWindow(estado.navigateSubwindow);
			reshapeNavigateSubwindow(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			redisplayAll();
		}
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

////////////////////////////////////
// Inicializações



void createDisplayLists(int janelaID)
{
	modelo.labirinto[janelaID] = glGenLists(2);
	glNewList(modelo.labirinto[janelaID], GL_COMPILE);
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
	desenhaLabirinto(modelo.texID[janelaID][ID_TEXTURA_CUBOS]);
	glPopAttrib();
	glEndList();

	modelo.chao[janelaID] = modelo.labirinto[janelaID] + 1;
	glNewList(modelo.chao[janelaID], GL_COMPILE);
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
	desenhaChao(CHAO_DIMENSAO, modelo.texID[janelaID][ID_TEXTURA_CHAO]);
	glPopAttrib();
	glEndList();
}


///////////////////////////////////
/// Texturas

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File = NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File = fopen(Filename, "r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

void createTextures(GLuint texID[])
{
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture
	char *image;
	int w, h, bpp;

	glGenTextures(NUM_TEXTURAS, texID);

	memset(TextureImage, 0, sizeof(void *) * 1);           	// Set The Pointer To NULL
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if (TextureImage[0] = LoadBMP(NOME_TEXTURA_CUBOS))
	{
		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CUBOS]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
	else
	{
		printf("Textura %s not Found\n", NOME_TEXTURA_CUBOS);
		exit(0);
	}

	if (read_JPEG_file(NOME_TEXTURA_CHAO, &image, &w, &h, &bpp))
	{
		glBindTexture(GL_TEXTURE_2D, texID[ID_TEXTURA_CHAO]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else {
		printf("Textura %s not Found\n", NOME_TEXTURA_CHAO);
		exit(0);

	}
	glBindTexture(GL_TEXTURE_2D, NULL);
}


void init()
{
	GLfloat amb[] = { 0.3f, 0.3f, 0.3f, 1.0f };

	estado.timer = 100;
	

	estado.camera.eye.x = 0;
	estado.camera.eye.y = OBJECTO_ALTURA * 2;
	estado.camera.eye.z = 0;
	estado.camera.dir_long = 0;
	estado.camera.dir_lat = 0;
	estado.camera.fov = 60;

	estado.localViewer = 1;
	estado.vista[JANELA_TOP] = 0;
	estado.vista[JANELA_NAVIGATE] = 0;

	modelo.objecto.pos.x = 0;
	modelo.objecto.pos.y = OBJECTO_ALTURA*.5;
	modelo.objecto.pos.z = 0;
	modelo.objecto.dir = 0;
	modelo.objecto.vel = OBJECTO_VELOCIDADE;

	modelo.xMouse = modelo.yMouse = -1;
	modelo.andar = GL_FALSE;
	modelo.km = 0;

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

void initNavigate()
{
	GLfloat amb[] = { 0.3f, 0.3f, 0.3f, 1.0f };

	estado.timer = 100;

	time_t timer = time(0);

	modelo.tempo = timer;

	estado.camera.eye.x = 0;
	estado.camera.eye.y = OBJECTO_ALTURA * 2;
	estado.camera.eye.z = 0;
	estado.camera.dir_long = 0;
	estado.camera.dir_lat = 0;
	estado.camera.fov = 60;

	estado.localViewer = 1;
	estado.vista[JANELA_TOP] = 0;
	estado.vista[JANELA_NAVIGATE] = 0;

	modelo.objecto.pos.x = 0;
	modelo.objecto.pos.y = OBJECTO_ALTURA*.5;
	modelo.objecto.pos.z = 0;
	modelo.objecto.dir = 0;
	modelo.objecto.vel = OBJECTO_VELOCIDADE;

	modelo.xMouse = modelo.yMouse = -1;
	modelo.andar = GL_FALSE;

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	clerigos.Load("Clerigos/clerigos.3ds");
	clerigos.lit = true;

	leGrafo();
}






/////////////////////////////////////
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(800 + GAP * 3, 400 + GAP * 2);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	if ((estado.mainWindow = glutCreateWindow("Labirinto")) == GL_FALSE)
		exit(1);
	imprime_ajuda();

	// Registar callbacks do GLUT da janela principal
	init();
	glutReshapeFunc(reshapeMainWindow);
	glutDisplayFunc(displayMainWindow);

	glutTimerFunc(estado.timer, Timer, 0);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);

	// criar a sub window 
	estado.topSubwindow = glutCreateSubWindow(estado.mainWindow, GAP, GAP, 400, 400);
	init();
	setLight();
	setMaterial();
	createTextures(modelo.texID[JANELA_TOP]);
	createDisplayLists(JANELA_TOP);

	mdlviewer_init("homer.mdl", modelo.homer[JANELA_TOP]);

	glutReshapeFunc(redisplayTopSubwindow);
	glutDisplayFunc(displayTopSubwindow);

	glutTimerFunc(estado.timer, Timer, 0);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);


	estado.navigateSubwindow = glutCreateSubWindow(estado.mainWindow, 400 + GAP, GAP, 400, 800);
	initNavigate();
	setLight();
	setMaterial();

	createTextures(modelo.texID[JANELA_NAVIGATE]);
	createDisplayLists(JANELA_NAVIGATE);
	mdlviewer_init("homer.mdl", modelo.homer[JANELA_NAVIGATE]);

	glutReshapeFunc(reshapeNavigateSubwindow);
	glutDisplayFunc(displayNavigateSubwindow);
	glutMouseFunc(mouseNavigateSubwindow);

	glutTimerFunc(estado.timer, Timer, 0);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);

	srand((unsigned)time(NULL));

	

	alutInit(&argc, argv);

	estado.buffer[0] = alutCreateBufferFromFile("anykey.wav");
	estado.buffer[1] = alutCreateBufferFromFile("the_days.wav");
	estado.buffer[2] = alutCreateBufferFromFile("boring.wav");
	estado.buffer[3] = alutCreateBufferFromFile("graduate.wav");
	estado.buffer[4] = alutCreateBufferFromFile("hmrcando.wav");
	estado.buffer[5] = alutCreateBufferFromFile("wahoo.wav");
	estado.buffer[6] = alutCreateBufferFromFile("bang_ow.wav");
	estado.buffer[7] = alutCreateBufferFromFile("doh2.wav");
	estado.buffer[8] = alutCreateBufferFromFile("dohaaa.wav");
	alGenSources(4, estado.source);
	alSourcei(estado.source[0], AL_BUFFER, estado.buffer[0]);
	alSourcePlay(estado.source[0]);

	glutMainLoop();
	return 0;
}
