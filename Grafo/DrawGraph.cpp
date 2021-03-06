#include "DrawGraph.h"



DrawGraph::DrawGraph()
{
}


DrawGraph::~DrawGraph()
{
}


void DrawGraph:: desenhaNo(Ponto noi, glTexture textura) {
	desenhaChaoRedondo(noi.largura*5, noi.longitude, noi.latitude, noi.altitude, textura);
}

void DrawGraph:: desenhaElemLigaFinal(Ligacao arco, glTexture textura) {

	Ponto noi = arco.destino;
	Ponto nof = arco.origem;
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

	desenhaChao(-si*0.5, -arco.largura*0.5, 0, si*0.5, arco.largura*0.5, 0, textura);
	glPopMatrix();

}



void DrawGraph:: desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, glTexture textura) {
	zi = zi - 0.0001;
	zf = zf - 0.0001;
	GLdouble v1[3], v2[3], cross[3];
	GLdouble length;
	v1[0] = xf - xi;
	v1[1] = 0;
	v2[0] = 0;
	v2[1] = yf - yi;

	v1[2] = 0;
	v2[2] = zf - zi;
	CrossProduct(v1, v2, cross);
	//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
	length = VectorNormalize(cross);
	//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textura.TextureID);
	//FAZER 1 FOR


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
}


void  DrawGraph::desenhaChaoRedondo(float largura, GLfloat x0, GLfloat y0, GLfloat z, glTexture textura) {
	double x, y;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textura.TextureID);

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
		glVertex3f(x, y, z + 0.01);
		ang += alfa;
	}

	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
	glDisable(GL_TEXTURE_2D);

}


void  DrawGraph:: CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[]){
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

GLdouble  DrawGraph:: VectorNormalize(GLdouble v[]){
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

void DrawGraph:: desenhaSolo() {
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

