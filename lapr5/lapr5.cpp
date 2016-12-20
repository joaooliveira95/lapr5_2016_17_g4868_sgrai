#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>
#include <GL/glaux.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

#include "grafos.h"
#include "som.h"
#include "Estado.h"
#include "Materials.h"
#include "Lights.h"
#include "DesenhaLabirinto.h"
#include "Model_3DS.h"
#include "Monumentos.h"

#pragma comment (lib, "glaux.lib")

typedef struct {
	Audio musica;
}Sounds;
Sounds sounds;

using namespace std;

#define graus(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)

typedef	GLdouble Vector[4];

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

Materials materials = Materials();
Estado estado = Estado();
Lights lights = Lights();
Modelo modelo;
Model_3DS truck,casaMusica;

void initEstado(){
	Cameras camera = Cameras();
	camera.setDir_Lat(M_PI / 4);
	camera.setDir_Long(-M_PI / 4);
	camera.setFov(60);
	camera.setDist(100);
	camera.setCenter(0, 0);
	camera.setCenter(1, 0);
	camera.setCenter(2, 0);
	
	estado.setCamera(camera);
	estado.setEixo(0, 0);
	estado.setEixo(1, 0);
	estado.setEixo(2, 0);

	estado.setLight(GL_TRUE);
	estado.setApresentaNormais(GL_FALSE);
	estado.setLightViewer(1);
}

void initModelo(){
	modelo.escala=0.2;
	modelo.cor_cubo = brass;
	modelo.g_pos_luz1[0]=-5.0;
	modelo.g_pos_luz1[1]= 5.0;
	modelo.g_pos_luz1[2]= 5.0;
	modelo.g_pos_luz1[3]= 0.0;
	modelo.g_pos_luz2[0]= 5.0;
	modelo.g_pos_luz2[1]= -15.0;
	modelo.g_pos_luz2[2]= 5.0;
	modelo.g_pos_luz2[3]= 0.0;
}

void myInit(){
	GLfloat LuzAmbiente[]={0.5,0.5,0.5, 0.0};
	glClearColor (0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	truck.Load("Clerigos/clerigos.3ds");
	truck.lit = true;

	casaMusica.Load("CasadaMusica/Casa+da+Musica.3ds");
	casaMusica.lit = true;

	glEnable(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_NORMALIZE);

	glDepthFunc(GL_LESS);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente); 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.getLightViewer()); 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 

	initModelo();
	initEstado();
	modelo.quad=gluNewQuadric();
	gluQuadricDrawStyle(modelo.quad, GLU_FILL);
	gluQuadricNormals(modelo.quad, GLU_OUTSIDE);

	leGrafo();
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
}

void desenhaPlanoDrag(int eixo){
	Cameras camera = estado.getCamera();
	glPushMatrix();
		glTranslated(estado.getEixo(0),estado.getEixo(1),estado.getEixo(2));
		switch (eixo) {
			case EIXO_Y :
					if(abs(camera.getDir_Lat())<M_PI/4)
						glRotatef(-90,0,0,1);
					else
						glRotatef(90,1,0,0);
					materials.material(red_plastic);
				break;
			case EIXO_X :
					if(abs(camera.getDir_Lat())>M_PI/6)
						glRotatef(90,1,0,0);
					materials.material(azul);
				break;
			case EIXO_Z :
					if(abs(cos(camera.getDir_Long()))>0.5)
						glRotatef(90,0,0,1);

					materials.material(emerald);
				break;
		}
		glBegin(GL_QUADS);
			glNormal3f(0,1,0);
			glVertex3f(-100,0,-100);
			glVertex3f(100,0,-100);
			glVertex3f(100,0,100);
			glVertex3f(-100,0,100);
		glEnd();
	glPopMatrix();
}

void displayNavigateWindow(void){
	DesenhaLabirinto dLabirinto = DesenhaLabirinto();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	estado.getCamera().setCamera(estado.isLight(), lights, modelo.g_pos_luz1, modelo.g_pos_luz2);
	
	glPushMatrix();
	materials.material(slate);
	dLabirinto.desenhaSolo();
	dLabirinto.desenhaEixos(estado.getEixo(0),estado.getEixo(1), estado.getEixo(2), modelo.quad);
	dLabirinto.desenhaLabirinto(numNos, numArcos, estado.isApresentaNormais());
	glPopMatrix();

	if(estado.getEixoTranslaccao()) {
		// desenha plano de translacção
		cout << "Translate... " << estado.getEixoTranslaccao() << endl; 
		desenhaPlanoDrag(estado.getEixoTranslaccao());

	}

	Monumentos monumentos = Monumentos();
	monumentos.desenhaClerigos(truck);
	monumentos.desenhaCasaMusica(casaMusica);

	glFlush();
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y){
    switch (key)
    {
		case 27 :
				exit(0);
			break;
		case 'h':
		case 'H':
				imprime_ajuda();
			break;
		case 'l':
		case 'L':
				if(estado.getLightViewer())
					estado.setLightViewer(0);
				else
					estado.setLightViewer(1);
				glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, estado.getLightViewer());
				glutPostRedisplay();
			break;
		case 'k':
		case 'K':
				estado.setLight(!estado.isLight());
				glutPostRedisplay();
			break;
		case 'w':
		case 'W':
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glutPostRedisplay();
			break;
		case 'p':
		case 'P':
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				glutPostRedisplay();
			break;
		case 's':
		case 'S':
				glEnable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glutPostRedisplay();
			break;
		case 'c':
		case 'C':
				if(glIsEnabled(GL_CULL_FACE))
					glDisable(GL_CULL_FACE);
				else
					glEnable(GL_CULL_FACE);
				glutPostRedisplay();
			break;    
		case 'n':
		case 'N':
				estado.setApresentaNormais(!estado.isApresentaNormais());
				glutPostRedisplay();
			break;    		
		case 'i':
		case 'I':
				initEstado();
				initModelo();
				glutPostRedisplay();
			break;    
	}
}

void Special(int key, int x, int y){
	Cameras camera = estado.getCamera();
	switch(key){
		case GLUT_KEY_F1 :
				gravaGrafo();
			break;
		case GLUT_KEY_F2 :
				leGrafo();
				glutPostRedisplay();
			break;	
		case GLUT_KEY_F6 :
				numNos=numArcos=0;
				addNo(criaNo( 0, 10,0));  // 0
				addNo(criaNo( 0,  5,0));  // 1
				addNo(criaNo(-5,  5,0));  // 2
				addNo(criaNo( 5,  5,0));  // 3
				addNo(criaNo(-5,  0,0));  // 4
				addNo(criaNo( 5,  0,0));  // 5
				addNo(criaNo(-5, -5,0));  // 6
				addArco(criaArco(0,1,1,1));  // 0 - 1
				addArco(criaArco(1,2,1,1));  // 1 - 2
				addArco(criaArco(1,3,1,1));  // 1 - 3
				addArco(criaArco(2,4,1,1));  // 2 - 4
				addArco(criaArco(3,5,1,1));  // 3 - 5
				addArco(criaArco(4,5,1,1));  // 4 - 5
				addArco(criaArco(4,6,1,1));  // 4 - 6
				glutPostRedisplay();
			break;	
		case GLUT_KEY_UP:
				camera.setDist(camera.getDist()-1);
				estado.setCamera(camera);
				glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
				camera.setDist(camera.getDist() + 1);
				estado.setCamera(camera);
				glutPostRedisplay();
			break;	
	}
}

void setProjection(int x, int y, GLboolean picking){
	Cameras camera = estado.getCamera();
    glLoadIdentity();
	if (picking) { // se está no modo picking, lê viewport e define zona de picking
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 4, 4, vport); // Inverte o y do rato para corresponder à jana
	}
	gluPerspective(camera.getFov(),(GLfloat)glutGet(GLUT_WINDOW_WIDTH) /glutGet(GLUT_WINDOW_HEIGHT) ,1,500);
}

void myReshape(int w, int h){	
	glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
	setProjection(0,0,GL_FALSE);
	glMatrixMode(GL_MODELVIEW);
}


void motionRotate(int x, int y){
	Cameras camera = estado.getCamera();
#define DRAG_SCALE	0.01
	double lim=M_PI/2-0.1;
	camera.setDir_Long(camera.getDir_Long() + (estado.getXMouse()-x)*DRAG_SCALE);
	camera.setDir_Lat(camera.getDir_Lat() - (estado.getYMouse()-y)*DRAG_SCALE*0.5);
	if(camera.getDir_Lat()>lim)
		camera.setDir_Lat(lim);
	else 
		if(camera.getDir_Lat()<-lim)
			camera.setDir_Lat(camera.getDir_Lat()-lim);

	estado.setXMouse(x);
	estado.setYMouse(y);
	estado.setCamera(camera);
	glutPostRedisplay();
}

void motionZoom(int x, int y){
	Cameras camera = estado.getCamera();
#define ZOOM_SCALE	0.5
	camera.setDist(camera.getDist() - (estado.getYMouse()-y)*ZOOM_SCALE);
	if(camera.getDist()<5)
		camera.setDist(5);
	else 
		if(camera.getDist()>200)
			camera.setDist(200);

	estado.setCamera(camera);
	estado.setYMouse(y);
	glutPostRedisplay();
}

void motionDrag(int x, int y){
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
		setProjection(x,y,GL_TRUE);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	estado.getCamera().setCamera(estado.isLight(), lights, modelo.g_pos_luz1, modelo.g_pos_luz2);
	desenhaPlanoDrag(estado.getEixoTranslaccao());
	
	n = glRenderMode(GL_RENDER);
	if (n > 0) {
		glGetIntegerv(GL_VIEWPORT, vp);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetDoublev(GL_MODELVIEW_MATRIX, mv);
		gluUnProject(x, glutGet(GLUT_WINDOW_HEIGHT) - y, (double) buffer[2] / UINT_MAX, mv, proj, vp, &newx, &newy, &newz);
		printf("Novo x:%lf, y:%lf, z:%lf\n\n", newx, newy, newz);
		switch (estado.getEixoTranslaccao()) {
			case EIXO_X :
					estado.setEixo(0, newx);
					//estado.eixo[1]=newy;
				break;
			case EIXO_Y :
					estado.setEixo(1, newy);
					//estado.eixo[2]=newz;
				break;
			case EIXO_Z :
					//estado.eixo[0]=newx;
					estado.setEixo(2, newz);
				break;		
		}
		glutPostRedisplay();
	}

	glMatrixMode(GL_PROJECTION); //repõe matriz projecção
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

int picking(int x, int y){
	int i, n, objid=0;
	double zmin = 10.0;
	GLuint buffer[100], *ptr;

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);
	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // guarda a projecção
		glLoadIdentity();
		setProjection(x,y,GL_TRUE);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	estado.getCamera().setCamera(estado.isLight(), lights, modelo.g_pos_luz1, modelo.g_pos_luz2);

	DesenhaLabirinto dLabirinto = DesenhaLabirinto();
	dLabirinto.desenhaEixos(estado.getEixo(0), estado.getEixo(1), estado.getEixo(2), modelo.quad);
	
	n = glRenderMode(GL_RENDER);
	if (n > 0)
	{
		ptr = buffer;
		for (i = 0; i < n; i++)
		{
			if (zmin > (double) ptr[1] / UINT_MAX) {
				zmin = (double) ptr[1] / UINT_MAX;
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
void mouse(int btn, int state, int x, int y){
	Cameras camera = estado.getCamera();
	switch(btn) {
		case GLUT_RIGHT_BUTTON :
					if(state == GLUT_DOWN){
						estado.setXMouse(x);
						estado.setYMouse(y);
						if(glutGetModifiers() & GLUT_ACTIVE_CTRL)
							glutMotionFunc(motionZoom);
						else
							glutMotionFunc(motionRotate);
						cout << "Left down\n";
					}
					else{
						glutMotionFunc(NULL);
						cout << "Left up\n";
					}
				break;
		case GLUT_LEFT_BUTTON :
					if(state == GLUT_DOWN){
						estado.setEixoTranslaccao(picking(x,y));
						if(estado.getEixoTranslaccao())
							glutMotionFunc(motionDrag);
						cout << "Right down - objecto:" << estado.getEixoTranslaccao() << endl;
					}
					else{
						if(estado.getEixoTranslaccao()!=0) {
							camera.setCenter(0, estado.getEixo(0));
							camera.setCenter(1, estado.getEixo(1));
							camera.setCenter(2, estado.getEixo(2));
							glutMotionFunc(NULL);
							estado.setEixoTranslaccao(0);
							estado.setCamera(camera);
							glutPostRedisplay();
						}
						cout << "Right up\n";
					}
				break;
	}
}

void main(int argc, char **argv){
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(displayNavigateWindow);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(mouse);

	alutInit(&argc, argv);
	Som som_c = Som();
	som_c.Backingtrack(sounds.musica);

	myInit();

	imprime_ajuda();
    glutMainLoop();
}
