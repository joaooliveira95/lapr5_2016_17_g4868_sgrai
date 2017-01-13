//SOURCE
#include "source.h"
#include "Grafo.h"
#include "OverlaysDesign.h"
#include "Skybox.h"
#include "ConsolaMenu.h"
#include "DrawGraph.h"
#include "DrawWeather.h"

using namespace std;

Estado estado;
Modelo modelo;
SKYBOX * skybox;
Texture textures;
InfoNo infoNo;
InfoArco infoArco;

GLfloat nextZ;

TextureLoader tl = TextureLoader();
Grafo grafo = Grafo();


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
	infoNo.i = 0;
	infoNo.isActive = GL_FALSE;
	infoArco.nome = "";
	infoArco.isActive = GL_FALSE;

	
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

	//Model_3DS m3ds = Model_3DS();
//	m3ds.Load("Modelos/casa_musica/CasaDaMusica.3ds");
	/*m3ds.pos.x = 0;
	m3ds.pos.y = 0;
	m3ds.pos.z = 0;
	m3ds.scale = 20;
	modelo.m3ds = m3ds;*/
}

void initParticles(int i) {
	par_sys[i].alive = true;
	par_sys[i].life = 1.0;
	par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)(rand() % 21) - 10;
	par_sys[i].ypos = (float)(rand() % 21) - 10;
	par_sys[i].zpos = (float)(rand() % 21) - 10;

	par_sys[i].red = 0.5;
	par_sys[i].green = 0.5;
	par_sys[i].blue = 1.0;

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -0.8;//-0.8;
}

void myInit(){
	modelo.objecto.pos.x = -50;
	modelo.objecto.pos.y = 0;
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
	tl.LoadTextureFromDisk("rotundaVisita.jpg", &textures.rotundaVisita);
	tl.LoadTextureFromDisk("info2.gif", &textures.info);

	//grafo.carregarGrafo("Porto");
	int x, z;

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	// Ground Verticies
	// Ground Colors
	for (z = 0; z < 21; z++) {
		for (x = 0; x < 21; x++) {
			ground_points[x][z][0] = x - 10.0;
			ground_points[x][z][1] = accum;
			ground_points[x][z][2] = z - 10.0;

			ground_colors[z][x][0] = r; // red value
			ground_colors[z][x][1] = g; // green value
			ground_colors[z][x][2] = b; // blue value
			ground_colors[z][x][3] = 0.0; // acummulation factor
		}
	}

	// Initialize particles
	for (loop = 0; loop < MAX_PARTICLES; loop++) {
		initParticles(loop);
	}

	fall = 4;
}

void material(enum tipo_material mat)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[mat]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[mat]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[mat]);
}

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
		eye[2] = modelo.objecto.pos.z + 0.5;

		estado.camera.center[0] = modelo.objecto.pos.x - sin(modelo.objecto.dir)*(0.25);
		estado.camera.center[1] = modelo.objecto.pos.y + cos(modelo.objecto.dir)*(0.25);
		estado.camera.center[2] = modelo.objecto.pos.z + 0.5;

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
	// Matriz onde s�o realizadas as tranformacoes dos modelos desenhados
	glMatrixMode(GL_MODELVIEW);
}

void redisplayAll(void){
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

void desenhaArco(Ligacao arco, glTexture textura, StudioModel candeeiro) {

	Ponto noi = arco.origem;
	Ponto nof = arco.destino;

	float si = K_LIGACAO * noi.largura;
	float sf = K_LIGACAO * nof.largura;

	float xi = noi.longitude;
	float xf = nof.longitude;
	float yi = noi.latitude;
	float yf = nof.latitude;
	float zi = noi.altitude;
	float zf = nof.altitude;

	float comp_p = sqrt(pow(xf - xi, 2) + pow(yf - yi, 2)) - si - sf;
	float desnivel_h = nof.altitude - noi.altitude;
	float comprimento_sif = sqrt(pow(comp_p, 2) + pow(desnivel_h, 2));
	float orientacao_a = atan2f((yf - yi), (xf - xi));
	float inclinacao_B = atan2f(desnivel_h, comp_p);

	glPushMatrix();
	glTranslatef(xi, yi, zi);

	glRotatef(graus(orientacao_a), 0, 0, 1);
	glTranslatef(si, 0, 0);
	glRotatef(graus(-inclinacao_B), 0, 1, 0);
	glTranslatef(comprimento_sif / 2.0, 0, 0);
	DrawGraph dGraph = DrawGraph();
	dGraph.desenhaChao(-comprimento_sif*0.5, -arco.largura*0.5, 0, comprimento_sif*0.5, arco.largura*0.5, 0, textura);


	GLfloat inc = comprimento_sif / 3;

	for (int i = 0; i <= 3; i++) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTranslatef(-comprimento_sif*0.5+inc*i, -arco.largura*0.5, 0);
		glScalef(0.01, 0.01, 0.01);
		glRotatef(graus(inclinacao_B), 0, 1, 0);
		mdlviewer_display(candeeiro);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	for (int i = 1; i <= 3; i++) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTranslatef(-comprimento_sif*0.5 + inc*i, arco.largura*0.5, 0);
		glScalef(-0.01, -0.01, 0.01);
		glRotatef(graus(-inclinacao_B), 0, 1, 0);
		mdlviewer_display(candeeiro);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	
	glPopMatrix();
}


GLboolean inAreaNo(Ponto no, GLfloat ny, GLfloat nx, int i) {
	float largura =no.largura;
	float x = no.longitude;
	float y = no.latitude;
	float z = no.altitude;
	
	//Valida se o Homer est� dentro do n� de raio = largura, centrado em (X,y)
	//Se estiver devolve True e atualiza nextZ ( altura do Homer)
	if(pow(nx - x,2) + pow(ny-y,2) <= pow(largura,2)){
		modelo.objecto.pos.y = ny;
		modelo.objecto.pos.x = nx;
		modelo.objecto.pos.z = z + OBJECTO_ALTURA/2;

		infoNo.i = i;
		infoNo.isActive = GL_TRUE;
		return true;
	}

	infoNo.i = i;
	infoNo.isActive = GL_FALSE;
	return false;
}

//Valida se o Homer esta dentro de um arco
GLboolean inAreaArco(Ligacao arco, GLfloat x1P, GLfloat y1P) {
	OverlaysDesign ui = OverlaysDesign();
	Ponto noi = arco.origem;
	Ponto nof = arco.destino;

	float si = K_LIGACAO * noi.largura;
	float sf = K_LIGACAO * nof.largura;

	float xi = noi.longitude;
	float xf = nof.longitude;
	float yi = noi.latitude;
	float yf = nof.latitude;
	float zi = noi.altitude;

	float orientacao_a = atan2f((yf - yi), (xf - xi));
	float comp_p = sqrt(pow(xf - xi, 2) + pow(yf - yi, 2)) - si - sf;
	float desnivel_h = nof.altitude - noi.altitude;


	float x2P = (x1P - xi) * cos(orientacao_a) + (y1P - yi) * sin(orientacao_a);
	float y2P = (y1P - yi) * cos(orientacao_a) - (x1P - xi) * sin(orientacao_a);

	float largura = arco.largura / 2.0;
	if (si <= x2P && x2P <= si + comp_p) {
		if (-largura <= y2P && y2P <= largura) {
			modelo.objecto.pos.x = x1P;
			modelo.objecto.pos.y = y1P;
			modelo.objecto.pos.z = zi + (x2P - si) / comp_p * desnivel_h + OBJECTO_ALTURA / 2;

			infoArco.nome = arco.nome;
			infoArco.isActive = GL_TRUE;
			return true;
		}
	}

	infoArco.nome = "";
	infoArco.isActive = GL_FALSE;
	return false;
}

boolean inAreaElemLiga(Ligacao arco, GLfloat x1P, GLfloat y1P) {
	Ponto noi = arco.origem;
	Ponto nof = arco.destino;

	float si = K_LIGACAO * noi.largura;
	float xi = noi.longitude;
	float xf = nof.longitude;
	float yi = noi.latitude;
	float yf = nof.latitude;
	float zi = noi.altitude;
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

boolean inAreaElemLigaFinal(Ligacao arco, GLfloat x1P, GLfloat y1P) {
	Ponto noi = arco.destino;
	Ponto nof = arco.origem;

	float si = K_LIGACAO * noi.largura;
	float xi = noi.longitude;
	float xf = nof.longitude;
	float yi = noi.latitude;
	float yf = nof.latitude;
	float zi = noi.altitude;
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


//Valida se o Homer se encontra fora das �reas permitidas
GLboolean detectaColisao(GLfloat nx, GLfloat nz) {
	for (int i = 0; i < grafo.quantidadePontos(); i++) {
		if (inAreaNo(grafo.obterPonto(i), nx, nz, i)) {
			return(GL_FALSE);
		}
	}

	for (int i = 0; i < grafo.quantidadeLigacoes(); i++) {
		if (inAreaArco(grafo.obterLigacao(i), nz, nx)) {
			return(GL_FALSE);
		}
		if (inAreaElemLiga(grafo.obterLigacao(i), nz, nx)) {
			return (GL_FALSE);
		}
		if (inAreaElemLigaFinal(grafo.obterLigacao(i), nz, nx)) {
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

	if (modelo.personagem.GetSequence() != 20){
		if (andar && modelo.personagem.GetSequence() != 3){
			modelo.personagem.SetSequence(3);
		}else if (!andar && modelo.personagem.GetSequence() != 0){
				modelo.personagem.SetSequence(0);
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
	printf("n,N - Liga/Desliga apresenta��o das normais \n");
	printf("******* grafos ******* \n");
	printf("F1  - Grava grafo do ficheiro \n");
	printf("F2  - L� grafo para ficheiro \n");
	printf("F6  - Cria novo grafo\n");
	printf("******* Camera ******* \n");
	printf("Bot�o esquerdo - Arrastar os eixos (centro da camera)\n");
	printf("Bot�o direito  - Rodar camera\n");
	printf("Bot�o direito com CTRL - Zoom-in/out\n");
	printf("PAGE_UP, PAGE_DOWN - Altera dist�ncia da camara \n");
	printf("ESC - Sair\n");
	//listNos();
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

int getModel(string nome) {
	for (int i = 0; i < NUM_MODELS_POIS; i++) {
		if (nome == modelo.poi[i].nome) {
			return i;
		}
	}
	return -1;
}

void desenhaModelo(int i) {
	int id = getModel(grafo.obterPonto(i).nome);
	if (id != -1) {
		mdlviewer_display(modelo.poi[id].model);
	}else if (id == ESCOLA_ID) {
		glScalef(0.05, 0.05, 0.05);
		glTranslatef(5.0, 5.0, 0);
		mdlviewer_display(modelo.poi[id].model);
	}else {
		glRotatef(180, 1, 0, 0);
		glTranslatef(0, 0, modelo.poi[PREDIO_ID].altitude);
		mdlviewer_display(modelo.poi[PREDIO_ID].model);
	}
}


void desenhaElemLigaInicial(Ligacao arco, glTexture textura, StudioModel stop) {

	Ponto noi = arco.origem;
	Ponto nof = arco.destino;
	float si = K_LIGACAO * noi.largura;
	float xi = noi.longitude;
	float xf = nof.longitude;
	float yi = noi.latitude;
	float yf = nof.latitude;
	float zi = noi.altitude;
	float zf = nof.altitude;

	float orientacao_a = atan2f((yf - yi), (xf - xi));

	glPushMatrix();
	glTranslatef(xi, yi, zi);
	glRotatef(graus(orientacao_a), 0, 0, 1);
	glTranslatef(si / 2, 0, 0);

	glPushMatrix();
	glColor3f(2, 2, 2);
	glEnable(GL_TEXTURE_2D);
	
	glTranslatef(si*0.5, arco.largura*0.5, 0);
	glScalef(0.02, 0.02, 0.02);
	//mdlviewer_display(modelo.poi[id].model);
	mdlviewer_display(stop);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	DrawGraph dGraph = DrawGraph();
	dGraph.desenhaChao(-si*0.5, -arco.largura*0.5, 0, si*0.5, arco.largura*0.5, 0, textura);
	glPopMatrix();

}


void desenhaGrafo() {
	DrawGraph dGraph = DrawGraph();
	material(cinza);
	glPushMatrix();
	glTranslatef(0, 0, 0.05);

	for (int i = 0; i<grafo.quantidadePontos(); i++) {
		glPushMatrix();
		glTranslatef(grafo.obterPonto(i).longitude, grafo.obterPonto(i).latitude, grafo.obterPonto(i).altitude);
		glPushMatrix();
		glScalef(0.005, 0.005, 0.005);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

		//desenhaModelo(i);
		//glPushMatrix();
		//modelo.m3ds.Draw();
		//glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glPopMatrix();
		glPopMatrix();
		Ponto no = grafo.obterPonto(i);
		if (no.visitar) {
			dGraph.desenhaNo(no, textures.rotundaVisita);
		}
		else {
			dGraph.desenhaNo(no, textures.rotunda);
		}

	}


	for (int i = 0; i < grafo.quantidadeLigacoes(); i++) {
		desenhaArco(grafo.obterLigacao(i), textures.chao, modelo.poi[CANDEEIRO_ID].model);
		desenhaElemLigaInicial(grafo.obterLigacao(i), textures.chao, modelo.poi[STOP_ID].model);
		dGraph.desenhaElemLigaFinal(grafo.obterLigacao(i), textures.chao);

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

// For Rain
void drawRain() {
	float x, y, z;
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glColor3f(0.5, 0.5, 0.8);
			glBegin(GL_LINES);
			glVertex3f(x, y, z);
			glVertex3f(x, y + 0.5, z);
			glEnd();

			// Update values
			//Move
			// Adjust slowdown for speed!
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].life = -1.0;
			}
			//Revive 
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}

// For Hail
void drawHail() {
	float x, y, z;

	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glColor3f(0.8, 0.8, 0.9);
			glBegin(GL_QUADS);
			// Front
			glVertex3f(x - hailsize, y - hailsize, z + hailsize); // lower left
			glVertex3f(x - hailsize, y + hailsize, z + hailsize); // upper left
			glVertex3f(x + hailsize, y + hailsize, z + hailsize); // upper right
			glVertex3f(x + hailsize, y - hailsize, z + hailsize); // lower left
																  //Left
			glVertex3f(x - hailsize, y - hailsize, z + hailsize);
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z + hailsize);
			// Back
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			//Right
			glVertex3f(x + hailsize, y + hailsize, z + hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z + hailsize);
			//Top 
			glVertex3f(x - hailsize, y + hailsize, z + hailsize);
			glVertex3f(x - hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z - hailsize);
			glVertex3f(x + hailsize, y + hailsize, z + hailsize);
			//Bottom 
			glVertex3f(x - hailsize, y - hailsize, z + hailsize);
			glVertex3f(x - hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z - hailsize);
			glVertex3f(x + hailsize, y - hailsize, z + hailsize);
			glEnd();

			// Update values
			//Move
			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].vel = par_sys[loop].vel * -1.0;
			}
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000); // * 1000
			par_sys[loop].vel += par_sys[loop].gravity;

			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			//Revive 
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
}

// For Snow
void drawSnow() {
	float x, y, z;
	for (loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;

			// Draw particles
			glColor3f(1.0, 1.0, 1.0);
			glPushMatrix();
			glTranslatef(x, y, z);
			glutSolidSphere(0.03, 16, 16);
			glPopMatrix();

			// Update values
			//Move
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			if (par_sys[loop].ypos <= -10) {
				int zi = z - zoom + 10;
				int xi = x + 10;
				ground_colors[zi][xi][0] = 1.0;
				ground_colors[zi][xi][2] = 1.0;
				ground_colors[zi][xi][3] += 1.0;
				if (ground_colors[zi][xi][3] > 1.0) {
					ground_points[xi][zi][1] += 0.1;
				}
				par_sys[loop].life = -1.0;
			}

			//Revive 
			if (par_sys[loop].life < 0.0) {
				initParticles(loop);
			}
		}
	}
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

	material(cinza);
	//desenhaSolo();
	//desenhaEixos();
	desenhaGrafo();
	if (estado.eixoTranslaccao) {
		// desenha plano de translac��o
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

	material(cinza);

	// Which Particles
	if (fall == RAIN) {
		material(azul);
		drawRain();
	}else if (fall == HAIL) {
		material(cinza);
		drawHail();
	}else if (fall == SNOW) {
		material(cinza);
		drawSnow();
	}

	material(cinza);
	if (infoNo.isActive) {
		ui.infoOverlay(grafo.obterPonto(infoNo.i).nome.c_str(), grafo.obterPonto(infoNo.i).descricao.c_str(), grafo.obterPonto(infoNo.i).abertura.c_str(), grafo.obterPonto(infoNo.i).fecho.c_str(), textures.info);
	}

	if (infoArco.isActive) {
		material(azul);
		ui.desenhaNomeRua(infoArco.nome);
	}

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
	case '1':
		fall = RAIN;
		glutPostRedisplay();
		break;
	case '2':
		fall = HAIL;
		glutPostRedisplay();
		break;
	case '3':
		fall = SNOW;
		glutPostRedisplay();
		break;
	case '4':
		fall = 6;
		glutPostRedisplay();
		break;
	}	
}

void Special(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_F1:
		//gravaGrafo();

		break;
/*	case GLUT_KEY_F2:
		leGrafo();
		redisplayAll();
		break;*/

	case GLUT_KEY_F3:

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
	case GLUT_KEY_PAGE_UP:
		if (estado.camera.dist<15)
			estado.camera.dist++;
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (estado.camera.dist>5)
			estado.camera.dist--;
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
	if (picking) { // se est� no modo picking, l� viewport e define zona de picking
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);
		gluPickMatrix(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 4, 4, vport); // Inverte o y do rato para corresponder � jana
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
	if (!estado.isFP) {
		double lim = M_PI / 2 - 0.1;
		estado.camera.dir_long += (estado.xMouse - x)*DRAG_SCALE;
		estado.camera.dir_lat -= (estado.yMouse - y)*DRAG_SCALE*0.5;
		if (estado.camera.dir_lat > lim)
			estado.camera.dir_lat = lim;
		else
			if (estado.camera.dir_lat < -lim)
				estado.camera.dir_lat = -lim;
		estado.xMouse = x;
		estado.yMouse = y;
		redisplayAll();
	}
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
	glPushMatrix(); // guarda a projec��o
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

	glMatrixMode(GL_PROJECTION); //rep�e matriz projec��o
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
	glPushMatrix(); // guarda a projec��o
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
			ptr += 3 + ptr[0]; // ptr[0] contem o n�mero de nomes (normalmente 1); 3 corresponde a numnomes, zmin e zmax
		}
	}
	glMatrixMode(GL_PROJECTION); //rep�e matriz projec��o
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

void initModelos() {
	modelo.poi[PREDIO_ID].altitude = 1.1;
	modelo.poi[PONTE_ID].altitude = -0.85;
	modelo.poi[ESCOLA_ID].altitude = -1.09;
	modelo.poi[COMBOIO_ID].altitude = -0.53;
	modelo.poi[FABRICA_ID].altitude = -1.2;
	modelo.poi[MAGAZIN_ID].altitude = -1.2;
	modelo.poi[MAGAZIN_ID].altitude = -1.2;

	mdlviewer_init("Modelos/magazin.mdl", modelo.poi[MAGAZIN_ID].model);
	modelo.poi[MAGAZIN_ID].nome = "Ponto5";
	mdlviewer_init("Modelos/escola.mdl", modelo.poi[ESCOLA_ID].model);
	modelo.poi[ESCOLA_ID].nome = "Ponto4";
	mdlviewer_init("Modelos/fabrica.mdl", modelo.poi[FABRICA_ID].model);
	modelo.poi[FABRICA_ID].nome = "Ponto3";
	mdlviewer_init("Modelos/ponte.mdl", modelo.poi[PONTE_ID].model);
	modelo.poi[PONTE_ID].nome = "Ponto1";
	mdlviewer_init("Modelos/predio.mdl", modelo.poi[PREDIO_ID].model);
	modelo.poi[PREDIO_ID].nome = "predio";
	mdlviewer_init("Modelos/comboio.mdl", modelo.poi[COMBOIO_ID].model);
	modelo.poi[COMBOIO_ID].nome = "Ponto2";

	mdlviewer_init("Modelos/stop_sign.mdl", modelo.poi[STOP_ID].model);
	mdlviewer_init("Modelos/streetLight.mdl", modelo.poi[CANDEEIRO_ID].model);
		
}

int main(int argc, char **argv){
	
	ConsolaMenu consola = ConsolaMenu();
	Grafo grafor = consola.consolaMain(grafo);
	grafo = grafor;

	alutInit(&argc, argv);
	estado.musica.buffer = alutCreateBufferFromFile("music.wav");
	alGenSources(1, &estado.musica.source);
	alSourcei(estado.musica.source, AL_BUFFER, estado.musica.buffer);
	alSourcef(estado.musica.source, AL_GAIN, 0.2);
	alSourcePlay(estado.musica.source);

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
	initModelos();

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
	initModelos();
	imprime_ajuda();

	skybox = new SKYBOX();
	if (skybox->Initialize()){
		// In�cio da aplica��o
		glutMainLoop();

		// Destrui��o da skybox
		skybox->Finalize();
		delete skybox;

		return 0;
	}
	return 1;
}