#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glaux.h>
#include <GL/glut.h>
#include "Model_3DS.h"
#include "DesenhaLabirinto.h"
#include "Estado.h"

#pragma comment (lib, "glaux.lib")

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define DEBUG 1

/* VARIAVEIS GLOBAIS */

typedef struct {
  GLboolean   doubleBuffer;
  GLint       delay;
}Estado1;

typedef struct {
  GLfloat     ang_h;
  GLfloat     ang_v;
  GLfloat     dist;
}Camara;

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
}Modelo;

Modelo modelo;
Estado cenas = Estado();
Estado1 estado;
Camara camara;
Model_3DS truck;

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
	modelo.quad = gluNewQuadric();
}


void initEstado() {
	cenas.setEixo(0, 0);
	cenas.setEixo(1, 0);
	cenas.setEixo(2, 0);

	cenas.setLight(GL_TRUE);
	cenas.setApresentaNormais(GL_FALSE);
	cenas.setLightViewer(1);
}

/* Inicialização do ambiente OPENGL */
void Init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  camara.ang_h = 45.0;
  camara.ang_v = 40.0;
  camara.dist = 20.0;

  initEstado();
  initModelo();

  truck.Load("Freightliner-truck1/Freightliner Aerodyne.3ds");
  truck.lit = true;


}

/**************************************
***  callbacks de janela/desenho    ***
**************************************/

// CALLBACK PARA REDIMENSIONAR JANELA

void Reshape(int width, int height)
{
  GLint size;

  if (width < height)
    size = width;
  else
    size = height;

  // glViewport(botom, left, width, height)
  // define parte da janela a ser utilizada pelo OpenGL

  glViewport(0, 0, (GLint) size, (GLint) size);
  

  // Matriz Projeccao
  // Matriz onde se define como o mundo e apresentado na janela
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // gluOrtho2D(left,right,bottom,top); 
  // projeccao em perspectiva
  gluPerspective(45.0, 1.0, 1.0, 100.0);

  // Matriz Modelview
  // Matriz onde são realizadas as tranformacoes dos modelos desenhados
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


// ... definicao das rotinas auxiliares de desenho ...

// Callback de desenho

void Draw(void){
DesenhaLabirinto dLabirinto = DesenhaLabirinto();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// ... transformacoes de visualizacao ...
  glPushMatrix();
  glTranslatef(0.0, 0.0, -camara.dist);
  glRotatef(camara.ang_v, 1.0, 0.0, 0.0);
  glRotatef(camara.ang_h, 0.0, 1.0, 0.0);
// ... chamada das rotinas auxiliares de desenho ...
  truck.Draw();
  
  dLabirinto.desenhaSolo();
  dLabirinto.desenhaEixos(cenas.getEixo(0), cenas.getEixo(1), cenas.getEixo(2), modelo.quad);
  dLabirinto.desenhaLabirinto(numNos, numArcos, cenas.isApresentaNormais());

  glPopMatrix();
  glFlush();
  if (estado.doubleBuffer)
    glutSwapBuffers();
}

/*******************************
***   callbacks timer/idle   ***
*******************************/

// Callback Idle

void Idle(void)
{

  // accoes a tomar quando o glut está idle 

  // redesenhar o ecra 
  // glutPostRedisplay();
}

// Callback de temporizador

void Timer(int value)
{
  glutTimerFunc(estado.delay, Timer, 0);
  // ... accoes do temporizador ... 


  // redesenhar o ecra 
  glutPostRedisplay();
}


void imprime_ajuda(void)
{
  printf("\n\nImportacao de um modelo 3DS\n");
  printf("h,H - Ajuda \n");
  printf("Setas  - Rotacao da camara\n");
  printf("PageUp,PageDown - Distancia da camara ao modelo\n");
  printf("ESC - Sair\n");
}

/*******************************
***  callbacks de teclado    ***
*******************************/

// Callback para interaccao via teclado (carregar na tecla)

void Key(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(1);
    // ... accoes sobre outras teclas ... 

    case 'h' :
    case 'H' :
                imprime_ajuda();
            break;
  }

  if(DEBUG)
    printf("Carregou na tecla %c\n",key);

}

// Callback para interaccao via teclado (largar a tecla)

/*void KeyUp(unsigned char key, int x, int y)
{

  if(DEBUG)
    printf("Largou a tecla %c\n",key);
}*/

// Callback para interaccao via teclas especiais  (carregar na tecla)

void SpecialKey(int key, int x, int y)
{
  // ... accoes sobre outras teclas especiais ... 
  //    GLUT_KEY_F1 ... GLUT_KEY_F12
  //    GLUT_KEY_UP
  //    GLUT_KEY_DOWN
  //    GLUT_KEY_LEFT
  //    GLUT_KEY_RIGHT
  //    GLUT_KEY_PAGE_UP
  //    GLUT_KEY_PAGE_DOWN
  //    GLUT_KEY_HOME
  //    GLUT_KEY_END
  //    GLUT_KEY_INSERT 

  switch (key) {
	case GLUT_KEY_LEFT:
		camara.ang_h -= 5;
		break;
	case GLUT_KEY_RIGHT:
		camara.ang_h += 5;
		break;
	case GLUT_KEY_DOWN:
		camara.ang_v -= 5;
		break;
	case GLUT_KEY_UP:
		camara.ang_v += 5;
		break;
	case GLUT_KEY_PAGE_UP:
		camara.dist /= 1.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		camara.dist *= 1.1;
		break;
  }
  if(DEBUG)
    printf("angH = %f graus, angV = %f graus, Dist = %f\n", camara.ang_h, camara.ang_v, camara.dist);
  // redesenhar o ecra 
  glutPostRedisplay();

  if(DEBUG)
    printf("Carregou na tecla especial %d\n",key);
}

// Callback para interaccao via teclas especiais (largar na tecla)

/*void SpecialKeyUp(int key, int x, int y)
{

  if(DEBUG)
    printf("Largou a tecla especial %d\n",key);

}*/


void main(int argc, char **argv)
{
  estado.doubleBuffer=GL_TRUE;

  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(600, 600);
  glutInitDisplayMode((estado.doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  if (glutCreateWindow("Exemplo") == GL_FALSE)
    exit(1);

  Init();

  imprime_ajuda();

// Registar callbacks do GLUT
  
  // callbacks de janelas/desenho
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Draw);

  // Callbacks de teclado
  glutKeyboardFunc(Key);
  //glutKeyboardUpFunc(KeyUp);
  glutSpecialFunc(SpecialKey);
  //glutSpecialFunc(SpecialKeyUp);

  // callbacks timer/idle
  //glutTimerFunc(estado.delay, Timer, 0);
  //glutIdleFunc(Idle);

  
  // COMECAR...
  glutMainLoop();
}
