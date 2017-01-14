//SOURCE
#include "source.h"
#include "Grafo.h"
#include "OverlaysDesign.h"
#include "Skybox.h"
#include "ConsolaMenu.h"
#include "DrawGraph.h"

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
Weather weather = Weather();


void initEstado() {
	estado.isFP = GL_FALSE;
	estado.camera.dir_lat = M_PI / 6;
	estado.camera.dir_long = -M_PI/2;
	estado.camera.fov = 60;
	estado.camera.dist = 2;
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

	//Posicao inicial do Homer
	modelo.objecto.pos.x = grafo.obterPonto(0).longitude;
	modelo.objecto.pos.y = grafo.obterPonto(0).latitude;
	modelo.objecto.pos.z = grafo.obterPonto(0).altitude + ALTURA_HOMER;

	modelo.objecto.dir = 0;
	modelo.objecto.vel = 0.0;
	modelo.andar = GL_FALSE;

	//Model_3DS m3ds = Model_3DS();
//	m3ds.Load("Modelos/casa_musica/CasaDaMusica.3ds");
	/*m3ds.pos.x = 0;
	m3ds.pos.y = 0;
	m3ds.pos.z = 0;
	m3ds.scale = 20;
	modelo.m3ds = m3ds;*/
}


void initWeather() {
	int x, z;

	// Initialize particles
	for (int loop = 0; loop < MAX_PARTICLES; loop++) {
		weather.initParticles(loop);
	}

	weather.setFall(4);
}

void initTexturas() {
	tl.LoadTextureFromDisk("calcada.jpg", &textures.chao);
	tl.LoadTextureFromDisk("rotunda.jpg", &textures.rotunda);
	tl.LoadTextureFromDisk("rotundaVisita.jpg", &textures.rotundaVisita);
	tl.LoadTextureFromDisk("info2.gif", &textures.info);
	tl.LoadTextureFromDisk("banda.jpg", &textures.banda);
}

void myInit(){

	//Guarda tempo inicial
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

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	initTexturas();
	initWeather();
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

	if (estado.isFP == GL_FALSE) {
		estado.camera.center[0] = modelo.objecto.pos.x;
		estado.camera.center[1] = modelo.objecto.pos.y;
		estado.camera.center[2]= modelo.objecto.pos.z;
		GLfloat eye[3];
		eye[0] = estado.camera.center[0] + estado.camera.dist*cos(estado.camera.dir_long)*cos(estado.camera.dir_lat);
		eye[1] = estado.camera.center[1] + estado.camera.dist*sin(estado.camera.dir_long)*cos(estado.camera.dir_lat);
		eye[2] = estado.camera.center[2]+ estado.camera.dist*sin(estado.camera.dir_lat);

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
		estado.camera.center[2] = modelo.objecto.pos.z+ ALTURA_HOMER;
		GLfloat eye[3];
		eye[0] = estado.camera.center[0] + estado.camera.dist*cos(rad(modelo.objecto.dir) + M_PI / 2)*cos(estado.camera.dir_lat);
		eye[1] = estado.camera.center[1] + estado.camera.dist*sin(rad(modelo.objecto.dir) + M_PI / 2)*cos(estado.camera.dir_lat);
		eye[2] = estado.camera.center[2] + estado.camera.dist*sin(estado.camera.dir_lat) + ALTURA_HOMER;

		if (estado.light) {
			gluLookAt(estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], eye[0], eye[1], eye[2], 0, 0, 1);
			putLights((GLfloat*)white_light);
		}
		else {
			putLights((GLfloat*)white_light);
			gluLookAt(estado.camera.center[0], estado.camera.center[1], estado.camera.center[2], eye[0], eye[1], eye[2], 0, 0, 1);
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

void redisplayAll(void){
	glutSetWindow(estado.mainWindow);
	glutPostRedisplay();
	glutSetWindow(estado.topSubwindow);
	glutPostRedisplay();
	glutSetWindow(estado.navigateSubwindow);
	glutPostRedisplay();
}

void displayMainWindow(){
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void desenhaCandeeiros(GLfloat comprimento_sif, GLfloat largura, GLfloat inclinacao_B, StudioModel candeeiro) {

	GLfloat inc = comprimento_sif / 3;

	for (int i = 0; i <= 3; i++) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTranslatef(-comprimento_sif*0.5 + inc*i, -largura, 0);
		glScalef(0.01, 0.01, 0.01);
		glRotatef(graus(inclinacao_B), 0, 1, 0);
		mdlviewer_display(candeeiro);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	for (int i = 1; i <= 3; i++) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTranslatef(-comprimento_sif*0.5 + inc*i, largura, 0);
		glScalef(-0.01, -0.01, 0.01);
		glRotatef(graus(-inclinacao_B), 0, 1, 0);
		mdlviewer_display(candeeiro);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
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

	desenhaCandeeiros(comprimento_sif, arco.largura*0.5, inclinacao_B, candeeiro);
	
	glPopMatrix();
}


GLboolean inAreaNo(Ponto no, GLfloat ny, GLfloat nx, int i) {
	float largura =no.largura;
	float x = no.longitude;
	float y = no.latitude;
	float z = no.altitude;
	
	//Valida se o Homer está dentro do nó de raio = largura, centrado em (X,y)
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


//Valida se o Homer se encontra fora das áreas permitidas
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

//Faz os calculos da colisao e da posicao do homer, recebendo como parametro a velocidade e o sinal(1-Tecla Up, -1-Tecla Down)
void teclaUpDown(int sinal) {
	GLfloat ny, nx;
	//Calcula a possivel nova posicao do homer
	ny = modelo.objecto.pos.y + cos(rad(modelo.objecto.dir))*modelo.objecto.vel * sinal;
	nx = modelo.objecto.pos.x + sin(rad(-modelo.objecto.dir))*modelo.objecto.vel * sinal;
	
	//Calcula a distancia percorrida até essa nova posicao
	GLfloat km_temp = sqrt(pow(ny - modelo.objecto.pos.y, 2) + pow(nx - modelo.objecto.pos.x, 2));
	//Deteta a colisao do homer
	if (!detectaColisao(ny, nx)) {
		
		//Atualiza a camara
		estado.camera.center[0] = modelo.objecto.pos.x;
		estado.camera.center[1] = modelo.objecto.pos.y;
		estado.camera.center[2] = modelo.objecto.pos.z;

		//Atualiza a distancia percorrida
		modelo.km += km_temp/100;
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
		if (modelo.objecto.vel<0.05) modelo.objecto.vel += 0.0005;
		teclaUpDown(1);
		andar = GL_TRUE;
	}
	if (estado.teclas.down) {
		if (modelo.objecto.vel<0.05) modelo.objecto.vel += 0.0005;
		teclaUpDown(-1);
		andar = GL_TRUE;
	}
	
	if (estado.teclas.left) {
		modelo.objecto.dir += 5;
		estado.camera.dir_long += rad(OBJECTO_ROTACAO);
	}
	if (estado.teclas.right) {
		modelo.objecto.dir -= 5;
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
		if (nome == modelo.modelos3D[i].nome) {
			return i;
		}
	}
	return -1;
}

void desenhaModelo(int i) {
	int id = getModel(grafo.obterPonto(i).nome);
	if (id != -1) {
		mdlviewer_display(modelo.modelos3D[id].model);
	}else if (id == ESCOLA_ID) {
		glScalef(0.05, 0.05, 0.05);
		glTranslatef(5.0, 5.0, 0);
		mdlviewer_display(modelo.modelos3D[id].model);
	}else {
		glRotatef(180, 1, 0, 0);
		glTranslatef(0, 0, modelo.modelos3D[PREDIO_ID].altitude);
		mdlviewer_display(modelo.modelos3D[PREDIO_ID].model);
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
	material(emerald);
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
		}else {
			dGraph.desenhaNo(no, textures.rotunda);
		}
	}

	for (int i = 0; i < grafo.quantidadeLigacoes(); i++) {
		desenhaArco(grafo.obterLigacao(i), textures.chao, modelo.modelos3D[CANDEEIRO_ID].model);
		desenhaElemLigaInicial(grafo.obterLigacao(i), textures.chao, modelo.modelos3D[STOP_ID].model);
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


void displayNavigateWindow(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	setCamera();
	putFoco();
	skybox->Render((float)abs(estado.camera.dir_long) * 6, (float)(estado.camera.dir_lat) * 6);

	glPushMatrix();
	glTranslatef(modelo.objecto.pos.x, modelo.objecto.pos.y, modelo.objecto.pos.z);
	glRotatef(modelo.objecto.dir, 0, 0, 1);
	glRotatef(90, 0, 0, 1);
	glScalef(SCALE_HOMER, SCALE_HOMER, SCALE_HOMER);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	mdlviewer_display(modelo.personagem);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	material(emerald);
	//desenhaSolo();
	//desenhaEixos();
	desenhaGrafo();
	if (estado.eixoTranslaccao) {
		// desenha plano de translacção
		cout << "Translate... " << estado.eixoTranslaccao << endl;
		desenhaPlanoDrag(estado.eixoTranslaccao);
	}
	OverlaysDesign ui = OverlaysDesign();
	ui.banda(textures.banda);
	//kms
	material(emerald);
	
	if (infoArco.isActive) {
		ui.desenhaNomeRua(infoArco.nome);
	}

	ui.desenhaKm(modelo.km);

	//Tempo
	time_t now = time(0);
	int tempoDecorrido = now - modelo.tempo;
	int mins = tempoDecorrido / 60;
	int segs = tempoDecorrido - 60 * mins;
	ui.desenhaTempo(mins, segs);	

	// Which Particles
	if (weather.getFall() == RAIN) {
		material(azul);
		weather.drawRain();
	}else if (weather.getFall() == HAIL) {
		material(cinza);
		weather.drawHail();
	}else if (weather.getFall() == SNOW) {
		material(emerald);
		weather.drawSnow();
	}

	material(emerald);
	if (infoNo.isActive) {
		ui.infoOverlay(grafo.obterPonto(infoNo.i).nome.c_str(), grafo.obterPonto(infoNo.i).descricao.c_str(), grafo.obterPonto(infoNo.i).abertura.c_str(), grafo.obterPonto(infoNo.i).fecho.c_str(), textures.info);
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
	DrawGraph dGraph = DrawGraph();
	dGraph.desenhaSolo();
	desenhaGrafo();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	ConsolaMenu cm = ConsolaMenu();
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'h':
	case 'H':
		cm.imprime_ajuda();
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
		weather.setFall(RAIN);
		glutPostRedisplay();
		break;
	case '2':
		weather.setFall(HAIL);
		glutPostRedisplay();
		break;
	case '3':
		weather.setFall(SNOW);
		glutPostRedisplay();
		break;
	case '4':
		weather.setFall(4);
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
		if (estado.camera.dist>1)
			estado.camera.dist--;
		break;
	}
}

// Callback para interaccao via teclas especiais (largar na tecla)
void SpecialKeyUp(int key, int x, int y){
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
		if (estado.isFP) {

		}else {
			estado.camera.dir_long += (estado.xMouse - x)*DRAG_SCALE;
		}
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

void initModelos() {
	modelo.modelos3D[PREDIO_ID].altitude = 1.1;
	modelo.modelos3D[PONTE_ID].altitude = -0.85;
	modelo.modelos3D[ESCOLA_ID].altitude = -1.09;
	modelo.modelos3D[COMBOIO_ID].altitude = -0.53;
	modelo.modelos3D[FABRICA_ID].altitude = -1.2;
	modelo.modelos3D[MAGAZIN_ID].altitude = -1.2;
	modelo.modelos3D[MAGAZIN_ID].altitude = -1.2;

	mdlviewer_init("Modelos/magazin.mdl", modelo.modelos3D[MAGAZIN_ID].model);
	modelo.modelos3D[MAGAZIN_ID].nome = "Ponto5";
	mdlviewer_init("Modelos/escola.mdl", modelo.modelos3D[ESCOLA_ID].model);
	modelo.modelos3D[ESCOLA_ID].nome = "Ponto4";
	mdlviewer_init("Modelos/fabrica.mdl", modelo.modelos3D[FABRICA_ID].model);
	modelo.modelos3D[FABRICA_ID].nome = "Ponto3";
	mdlviewer_init("Modelos/ponte.mdl", modelo.modelos3D[PONTE_ID].model);
	modelo.modelos3D[PONTE_ID].nome = "Ponto1";
	mdlviewer_init("Modelos/predio.mdl", modelo.modelos3D[PREDIO_ID].model);
	modelo.modelos3D[PREDIO_ID].nome = "predio";
	mdlviewer_init("Modelos/comboio.mdl", modelo.modelos3D[COMBOIO_ID].model);
	modelo.modelos3D[COMBOIO_ID].nome = "Ponto2";

	mdlviewer_init("Modelos/stop_sign.mdl", modelo.modelos3D[STOP_ID].model);
	mdlviewer_init("Modelos/streetLight.mdl", modelo.modelos3D[CANDEEIRO_ID].model);
		
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