#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>
#include "grafos.h"
#include "som.h"
#include "Estado.h"
#include "Materials.h"

typedef struct {
	Audio musica, crowd, KO, Punch, Fight, YouWin, Select;
}Sounds;

Sounds sounds;

using namespace std;

#define graus(X) (double)((X)*180/M_PI)
#define rad(X)   (double)((X)*M_PI/180)

// luzes e materiais



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
Modelo modelo;

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

	estado.setLight(GL_FALSE);
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

void imprime_ajuda(void)
{
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


const GLfloat red_light[] = {1.0, 0.0, 0.0, 1.0};
const GLfloat green_light[] = {0.0, 1.0, 0.0, 1.0};
const GLfloat blue_light[] = {0.0, 0.0, 1.0, 1.0};
const GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};


void putLights(GLfloat* diffuse)
{
	const GLfloat white_amb[] = {0.2, 0.2, 0.2, 1.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, modelo.g_pos_luz1);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_amb);
	glLightfv(GL_LIGHT1, GL_POSITION, modelo.g_pos_luz2);

	/* desenhar luz */
	//material(red_plastic);
	//glPushMatrix();
	//	glTranslatef(modelo.g_pos_luz1[0], modelo.g_pos_luz1[1], modelo.g_pos_luz1[2]);
	//	glDisable(GL_LIGHTING);
	//	glColor3f(1.0, 1.0, 1.0);
	//	glutSolidCube(0.1);
	//	glEnable(GL_LIGHTING);
	//glPopMatrix();
	//glPushMatrix();
	//	glTranslatef(modelo.g_pos_luz2[0], modelo.g_pos_luz2[1], modelo.g_pos_luz2[2]);
	//	glDisable(GL_LIGHTING);
	//	glColor3f(1.0, 1.0, 1.0);
	//	glutSolidCube(0.1);
	//	glEnable(GL_LIGHTING);
	//glPopMatrix();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void desenhaSolo(){
#define STEP 10
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		for(int i=-300;i<300;i+=STEP)
			for(int j=-300;j<300;j+=STEP){
				glVertex2f(i,j);
				glVertex2f(i+STEP,j);
				glVertex2f(i+STEP,j+STEP);
				glVertex2f(i,j+STEP);
			}
	glEnd();
}

void CrossProduct (GLdouble v1[], GLdouble v2[], GLdouble cross[]){
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

GLdouble VectorNormalize (GLdouble v[])
{
	int		i;
	GLdouble	length;

	if ( fabs(v[1] - 0.000215956) < 0.0001)
		i=1;

	length = 0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrt (length);
	if (length == 0)
		return 0;
		
	for (i=0 ; i< 3 ; i++)
		v[i] /= length;	

	return length;
}

void desenhaNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], tipo_material mat){

	switch (mat){
		case red_plastic:
				glColor3f(1,0,0);
			break;
		case azul:
				glColor3f(0,0,1);
			break;
		case emerald:
				glColor3f(0,1,0);
			break;
		default:
				glColor3f(1,1,0);
	}
	glDisable(GL_LIGHTING);
	glPushMatrix();
		glTranslated(x,y,z);
		glScaled(0.4,0.4,0.4);
		glBegin(GL_LINES);
			glVertex3d(0,0,0);
			glVertex3dv(normal);
		glEnd();
		glPopMatrix();
	glEnable(GL_LIGHTING);
}

void desenhaParede(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf){
	
	GLdouble v1[3],v2[3],cross[3];
	GLdouble length;
	v1[0]=xf-xi;
	v1[1]=yf-yi;
	v1[2]=0;
	v2[0]=0;
	v2[1]=0;
	v2[2]=1;
	CrossProduct(v1,v2,cross);
	//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
	length=VectorNormalize(cross);
	//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);

	materials.material(emerald);
	glBegin(GL_QUADS);
		glNormal3dv(cross);
		glVertex3f(xi,yi,zi);
		glVertex3f(xf,yf,zf+0);
		glVertex3f(xf,yf,zf+1);
		glVertex3f(xi,yi,zi+1);
	glEnd();

	if(estado.isApresentaNormais()) {
		desenhaNormal(xi,yi,zi,cross,emerald);
		desenhaNormal(xf,yf,zf,cross,emerald);
		desenhaNormal(xf,yf,zf+1,cross,emerald);
		desenhaNormal(xi,yi,zi+1,cross,emerald);
	}
}

void desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, int orient){
	GLdouble v1[3],v2[3],cross[3];
	GLdouble length;
	v1[0]=xf-xi;
	v1[1]=0;
	v2[0]=0;
	v2[1]=yf-yi;

	switch(orient) {
		case NORTE_SUL :
				v1[2]=0;
				v2[2]=zf-zi;
				CrossProduct(v1,v2,cross);
				//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
				length=VectorNormalize(cross);
				//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);

				materials.material(red_plastic);
				glBegin(GL_QUADS);
					glNormal3dv(cross);
					glVertex3f(xi,yi,zi);
					glVertex3f(xf,yi,zi);
					glVertex3f(xf,yf,zf);
					glVertex3f(xi,yf,zf);
				glEnd();
				if(estado.isApresentaNormais()) {
					desenhaNormal(xi,yi,zi,cross,red_plastic);
					desenhaNormal(xf,yi,zi,cross,red_plastic);
					desenhaNormal(xf,yf,zf,cross,red_plastic);
					desenhaNormal(xi,yi,zf,cross,red_plastic);
				}
			break;
		case ESTE_OESTE :
				v1[2]=zf-zi;
				v2[2]=0;
				CrossProduct(v1,v2,cross);
				//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
				length=VectorNormalize(cross);
				//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);
				materials.material(red_plastic);
				glBegin(GL_QUADS);
					glNormal3dv(cross);
					glVertex3f(xi,yi,zi);
					glVertex3f(xf,yi,zf);
					glVertex3f(xf,yf,zf);
					glVertex3f(xi,yf,zi);
				glEnd();
				if(estado.isApresentaNormais()) {
					desenhaNormal(xi,yi,zi,cross,red_plastic);
					desenhaNormal(xf,yi,zf,cross,red_plastic);
					desenhaNormal(xf,yf,zf,cross,red_plastic);
					desenhaNormal(xi,yi,zi,cross,red_plastic);
				}
			break;
		default:
				cross[0]=0;
				cross[1]=0;
				cross[2]=1;
				materials.material(azul);
				glBegin(GL_QUADS);
					glNormal3f(0,0,1);
					glVertex3f(xi,yi,zi);
					glVertex3f(xf,yi,zf);
					glVertex3f(xf,yf,zf);
					glVertex3f(xi,yf,zi);
				glEnd();
				if(estado.isApresentaNormais()) {
					desenhaNormal(xi,yi,zi,cross,azul);
					desenhaNormal(xf,yi,zf,cross,azul);
					desenhaNormal(xf,yf,zf,cross,azul);
					desenhaNormal(xi,yi,zi,cross,azul);
				}
			break;
	}
}

void desenhaNo(int no){
	GLboolean norte,sul,este,oeste;
	GLfloat larguraNorte,larguraSul,larguraEste,larguraOeste;
	Arco arco=arcos[0];
	No *noi=&nos[no],*nof;
	norte=sul=este=oeste=GL_TRUE;
	desenhaChao(nos[no].x-0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z,nos[no].x+0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z,PLANO);
	for(int i=0;i<numArcos; arco=arcos[++i]){
		if(arco.noi==no)
			nof=&nos[arco.nof];
		else 
			if(arco.nof==no)
				nof=&nos[arco.noi];
			else
				continue;
		if(noi->x==nof->x)
			if(noi->y<nof->y){
				norte=GL_FALSE;
				larguraNorte=arco.largura;
			}
			else{
				sul=GL_FALSE;
				larguraSul=arco.largura;
			}
		else 
			if(noi->y==nof->y)
				if(noi->x<nof->x){
					oeste=GL_FALSE;
					larguraOeste=arco.largura;
				}
				else{
					este=GL_FALSE;
					larguraEste=arco.largura;
				}
			else
				cout << "Arco dioagonal: " << arco.noi << " " << arco.nof << endl;
		if (norte && sul && este && oeste)
			return;
	}		
	if(norte)
		desenhaParede(nos[no].x-0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z,nos[no].x+0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z);
	else
		if (larguraNorte < noi->largura){
			desenhaParede(nos[no].x-0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z,nos[no].x-0.5*larguraNorte,nos[no].y+0.5*noi->largura,nos[no].z);
			desenhaParede(nos[no].x+0.5*larguraNorte,nos[no].y+0.5*noi->largura,nos[no].z,nos[no].x+0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z);
		}
	if(sul)
		desenhaParede(nos[no].x+0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z,nos[no].x-0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z);
	else
		if (larguraSul < noi->largura){
			desenhaParede(nos[no].x+0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z,nos[no].x+0.5*larguraSul,nos[no].y-0.5*noi->largura,nos[no].z);
			desenhaParede(nos[no].x-0.5*larguraSul,nos[no].y-0.5*noi->largura,nos[no].z,nos[no].x-0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z);
		}
	if(este)
		desenhaParede(nos[no].x-0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z,nos[no].x-0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z);
	else
		if (larguraEste < noi->largura){
			desenhaParede(nos[no].x-0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z,nos[no].x-0.5*noi->largura,nos[no].y-0.5*larguraEste,nos[no].z);
			desenhaParede(nos[no].x-0.5*noi->largura,nos[no].y+0.5*larguraEste,nos[no].z,nos[no].x-0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z);
		}
	if(oeste)
		desenhaParede(nos[no].x+0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z,nos[no].x+0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z);
	else
		if (larguraOeste < noi->largura){
			desenhaParede(nos[no].x+0.5*noi->largura,nos[no].y+0.5*noi->largura,nos[no].z,nos[no].x+0.5*noi->largura,nos[no].y+0.5*larguraOeste,nos[no].z);
			desenhaParede(nos[no].x+0.5*noi->largura,nos[no].y-0.5*larguraOeste,nos[no].z,nos[no].x+0.5*noi->largura,nos[no].y-0.5*noi->largura,nos[no].z);
		}
}


void desenhaArco(Arco arco){
	No *noi,*nof;

	if(nos[arco.noi].x==nos[arco.nof].x){
		// arco vertical
		if(nos[arco.noi].y<nos[arco.nof].y){
			noi=&nos[arco.noi];
			nof=&nos[arco.nof];
		}else{
			nof=&nos[arco.noi];
			noi=&nos[arco.nof];
		}

		desenhaChao(noi->x-0.5*arco.largura,noi->y+0.5*noi->largura,noi->z,nof->x+0.5*arco.largura,nof->y-0.5*nof->largura,nof->z, NORTE_SUL);
		desenhaParede(noi->x-0.5*arco.largura,noi->y+0.5*noi->largura,noi->z,nof->x-0.5*arco.largura,nof->y-0.5*nof->largura,nof->z);
		desenhaParede(nof->x+0.5*arco.largura,nof->y-0.5*nof->largura,nof->z,noi->x+0.5*arco.largura,noi->y+0.5*noi->largura,noi->z);
	}else{
		if(nos[arco.noi].y==nos[arco.nof].y){
			//arco horizontal
			if(nos[arco.noi].x<nos[arco.nof].x){
				noi=&nos[arco.noi];
				nof=&nos[arco.nof];
			}else{
				nof=&nos[arco.noi];
				noi=&nos[arco.nof];
			}
			desenhaChao(noi->x+0.5*noi->largura,noi->y-0.5*arco.largura,noi->z,nof->x-0.5*nof->largura,nof->y+0.5*arco.largura,nof->z, ESTE_OESTE);
			desenhaParede(noi->x+0.5*noi->largura,noi->y+0.5*arco.largura,noi->z,nof->x-0.5*nof->largura,nof->y+0.5*arco.largura,nof->z);
			desenhaParede(nof->x-0.5*nof->largura,nof->y-0.5*arco.largura,nof->z,noi->x+0.5*noi->largura,noi->y-0.5*arco.largura,noi->z);
		}
		else{
			cout << "arco diagonal... não será desenhado";
		}
	}
}

void desenhaLabirinto(){
	glPushMatrix();
		glTranslatef(0,0,0.05);
		glScalef(5,5,5);
		materials.material(red_plastic);
		for(int i=0; i<numNos; i++){
			glPushMatrix();
			materials.material(preto);
				glTranslatef(nos[i].x,nos[i].y,nos[i].z+0.25);
				glutSolidCube(0.5);
			glPopMatrix();
			desenhaNo(i);
		}
		materials.material(emerald);
		for(int i=0; i<numArcos; i++)
			desenhaArco(arcos[i]);
	glPopMatrix();
}

void desenhaEixo(){
	gluCylinder(modelo.quad,0.5,0.5,20,16,15);
	glPushMatrix();
		glTranslatef(0,0,20);
		glPushMatrix();
			glRotatef(180,0,1,0);
			gluDisk(modelo.quad,0.5,2,16,6);
		glPopMatrix();
		gluCylinder(modelo.quad,2,0,5,16,15);
	glPopMatrix();
}

#define EIXO_X		1
#define EIXO_Y		2
#define EIXO_Z		3

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

void desenhaEixos(){

	glPushMatrix();
		glTranslated(estado.getEixo(0),estado.getEixo(1),estado.getEixo(2));
		materials.material(emerald);
		glPushName(EIXO_Z);
			desenhaEixo();
		glPopName();
		glPushName(EIXO_Y);
			glPushMatrix();
				glRotatef(-90,1,0,0);
				materials.material(red_plastic);
				desenhaEixo();
			glPopMatrix();
		glPopName();
		glPushName(EIXO_X);
			glPushMatrix();
				glRotatef(90,0,1,0);
				materials.material(azul);
				desenhaEixo();
			glPopMatrix();
		glPopName();
	glPopMatrix();
}

void setCamera(){
	Vertice eye;
	Cameras camera = estado.getCamera();
	eye[0]=camera.getCenter(0)+camera.getDist()*cos(camera.getDir_Long())*cos(camera.getDir_Lat());
	eye[1]=camera.getCenter(1)+camera.getDist()*sin(camera.getDir_Long())*cos(camera.getDir_Lat());
	eye[2]=camera.getCenter(2)+camera.getDist()*sin(camera.getDir_Lat());

	if(estado.isLight()){
		gluLookAt(eye[0],eye[1],eye[2],camera.getCenter(0),camera.getCenter(1),camera.getCenter(2),0,0,1);
		putLights((GLfloat*)white_light);
	}else{
		putLights((GLfloat*)white_light);
		gluLookAt(eye[0],eye[1],eye[2],camera.getCenter(0),camera.getCenter(1),camera.getCenter(2),0,0,1);
	}
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	setCamera();

	materials.material(slate);
	desenhaSolo();

	
	desenhaEixos();
	
	desenhaLabirinto();
 
	if(estado.getEixoTranslaccao()) {
		// desenha plano de translacção
		cout << "Translate... " << estado.getEixoTranslaccao() << endl; 
		desenhaPlanoDrag(estado.getEixoTranslaccao());

	}
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
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT)  - y, 4, 4, vport); // Inverte o y do rato para corresponder à jana
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
	setCamera();
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
	setCamera();
	desenhaEixos();
	
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

void main(int argc, char **argv)
{
    glutInit(&argc, argv);

/* need both double buffering and z buffer */

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
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
