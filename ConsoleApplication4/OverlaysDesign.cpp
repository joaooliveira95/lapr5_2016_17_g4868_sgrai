#include "OverlaysDesign.h"


#define HEALTH_BAR_HEIGHT 20

OverlaysDesign::OverlaysDesign()
{
}


OverlaysDesign::~OverlaysDesign()
{
}


void OverlaysDesign::bitmapString(char *str, double x, double y) {
	int i, n;

	n = strlen(str);
	glRasterPos2d(x, y);
	for (i = 0; i<n; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)str[i]);
}

void OverlaysDesign::bitmapCenterString(char *str, double x, double y) {
	int i, n;
	n = strlen(str);
	glRasterPos2d(x - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)str)*0.5, y);
	for (i = 0; i<n; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)str[i]);
}


void OverlaysDesign::desenhaKm(float x) {
	char str[255];	

	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glLineWidth(3);
	glLineStipple(2, 0xFF00);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 20);
	glDisable(GL_DEPTH_TEST);
	glColor3f(2, 2, 2);

	sprintf_s(str, 255, "%.f Km", x);
	bitmapCenterString(str, width*0.8, height*0.9);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void OverlaysDesign::desenhaTempo(int seg,int min) {
	char str[255];
	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glLineWidth(3);
	glLineStipple(2, 0xFF00);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 20);
	glDisable(GL_DEPTH_TEST);
	glColor3f(2, 2, 2);

	sprintf_s(str, 255, "%d : %d", seg, min);
	bitmapCenterString(str, 0, height*0.9);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void OverlaysDesign::desenhaNomeRua(string rua) {
	char str[255];
	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glLineWidth(3);
	glLineStipple(2, 0xFF00);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 20);
	glDisable(GL_DEPTH_TEST);
	glColor3f(2, 2, 2);

	sprintf_s(str, 255, "Rua : %s", rua.c_str());
	bitmapCenterString(str, -width*0.8, height*0.9);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}

void OverlaysDesign::banda(glTexture textura) {
	char str[255];
	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glLineWidth(3);
	glLineStipple(2, 0xFF00);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 20);
	glDisable(GL_DEPTH_TEST);
	//glColor3f(4, 4, 4);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textura.TextureID);

	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0);
	glVertex2f( - width, height*0.85);//VERTICE
	glTexCoord2f(0, 1.0);
	glVertex2f(width, height*0.85);//VERTICE
	glTexCoord2f(0.0, 0.0);
	glVertex2f(width, height);//VERTICE
	glTexCoord2f(1.0, 0.0);
	glVertex2f(-width, height);//VERTICE
	glEnd();
	glDisable(GL_BLEND);
	glColor3f(4, 4, 4);

	//glPushAttrib(GL_CURRENT_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);
}

void OverlaysDesign::infoOverlay(const char* nome, const char* descricao, const char* abertura, const char* fecho, glTexture textura) {
	char str[255];
	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	glLineWidth(3);
	glLineStipple(2, 0xFF00);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-width, width, -height, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(20, 20);
	glDisable(GL_DEPTH_TEST);
	glColor3f(4, 4, 4);

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textura.TextureID);

	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(width*0.40, -height*0.98);//VERTICE
	glTexCoord2f(0, 1.0);
	glVertex2f(width*0.99, -height*0.98);//VERTICE
	glTexCoord2f(0.0, 0.0);
	glVertex2f(width*0.99, -height*0.25);//VERTICE
	glTexCoord2f(1.0, 0.0);
	glVertex2f(width*0.40, -height*0.25);//VERTICE
	glEnd();
	glDisable(GL_BLEND);
	glColor3f(4, 4, 4);
	sprintf_s(str, 255, "Nome: %s", nome);
	bitmapCenterString(str, width*0.70, -height*0.5);
	sprintf_s(str, 255, "Descricao: %s", descricao);
	bitmapCenterString(str, width*0.70, -height*0.6);
	sprintf_s(str, 255, "Hora da Abertura: %s ", abertura);
	bitmapCenterString(str, width*0.70, -height*0.7);
	sprintf_s(str, 255, " Hora do Fecho: %s \n", fecho);
	bitmapCenterString(str, width*0.70, -height*0.8);
		
	//glPushAttrib(GL_CURRENT_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

}

