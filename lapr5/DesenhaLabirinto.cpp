#include "DesenhaLabirinto.h"



DesenhaLabirinto::DesenhaLabirinto()
{
}


DesenhaLabirinto::~DesenhaLabirinto()
{
}

void DesenhaLabirinto:: desenhaSolo() {
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

void DesenhaLabirinto:: desenhaLabirinto(int numNos, int numArcos, GLboolean isApresentaNormais) {
	Materials materials = Materials();
	glPushMatrix();
	glTranslatef(0, 0, 5.05);
	glRotatef(270, 1, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	materials.material(red_plastic);
	for (int i = 0; i<numNos; i++) {
		glPushMatrix();
		materials.material(preto);
		glTranslatef(nos[i].x, nos[i].y, nos[i].z + 0.25);
		glutSolidCube(0.5);
		glPopMatrix();
		desenhaNo(i, isApresentaNormais);
	}
	materials.material(emerald);
	for (int i = 0; i<numArcos; i++)
		desenhaArco(arcos[i], isApresentaNormais);
	glPopMatrix();
}


void DesenhaLabirinto:: desenhaEixos(GLdouble eixo0, GLdouble eixo1, GLdouble eixo2, GLUquadric *quad) {
	Materials materials = Materials();
	glPushMatrix();
	glTranslated(eixo0, eixo1, eixo2);
	materials.material(emerald);
	glPushName(EIXO_Z);
	desenhaEixo(quad);
	glPopName();
	glPushName(EIXO_Y);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	materials.material(red_plastic);
	desenhaEixo(quad);
	glPopMatrix();
	glPopName();
	glPushName(EIXO_X);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	materials.material(azul);
	desenhaEixo(quad);
	glPopMatrix();
	glPopName();
	glPopMatrix();
}


void DesenhaLabirinto:: desenhaEixo(GLUquadric *quad) {
	gluCylinder(quad, 0.5, 0.5, 20, 16, 15);
	glPushMatrix();
	glTranslatef(0, 0, 20);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	gluDisk(quad, 0.5, 2, 16, 6);
	glPopMatrix();
	gluCylinder(quad, 2, 0, 5, 16, 15);
	glPopMatrix();
}



void DesenhaLabirinto:: desenhaArco(Arco arco, GLboolean isApresentaNormais) {
	No *noi, *nof;

	if (nos[arco.noi].x == nos[arco.nof].x) {
		// arco vertical
		if (nos[arco.noi].y<nos[arco.nof].y) {
			noi = &nos[arco.noi];
			nof = &nos[arco.nof];
		}
		else {
			nof = &nos[arco.noi];
			noi = &nos[arco.nof];
		}

		desenhaChao(noi->x - 0.5*arco.largura, noi->y + 0.5*noi->largura, noi->z, nof->x + 0.5*arco.largura, nof->y - 0.5*nof->largura, nof->z, NORTE_SUL, isApresentaNormais);
		desenhaParede(noi->x - 0.5*arco.largura, noi->y + 0.5*noi->largura, noi->z, nof->x - 0.5*arco.largura, nof->y - 0.5*nof->largura, nof->z, isApresentaNormais);
		desenhaParede(nof->x + 0.5*arco.largura, nof->y - 0.5*nof->largura, nof->z, noi->x + 0.5*arco.largura, noi->y + 0.5*noi->largura, noi->z, isApresentaNormais);
	}
	else {
		if (nos[arco.noi].y == nos[arco.nof].y) {
			//arco horizontal
			if (nos[arco.noi].x<nos[arco.nof].x) {
				noi = &nos[arco.noi];
				nof = &nos[arco.nof];
			}
			else {
				nof = &nos[arco.noi];
				noi = &nos[arco.nof];
			}
			desenhaChao(noi->x + 0.5*noi->largura, noi->y - 0.5*arco.largura, noi->z, nof->x - 0.5*nof->largura, nof->y + 0.5*arco.largura, nof->z, ESTE_OESTE, isApresentaNormais);
			desenhaParede(noi->x + 0.5*noi->largura, noi->y + 0.5*arco.largura, noi->z, nof->x - 0.5*nof->largura, nof->y + 0.5*arco.largura, nof->z, isApresentaNormais);
			desenhaParede(nof->x - 0.5*nof->largura, nof->y - 0.5*arco.largura, nof->z, noi->x + 0.5*noi->largura, noi->y - 0.5*arco.largura, noi->z, isApresentaNormais);
		}
		else {
		//	cout << "arco diagonal... não será desenhado";
		}
	}
}


void DesenhaLabirinto:: desenhaChao(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, int orient, GLboolean isApresentaNormais) {
	Materials materials = Materials();
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

		materials.material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zi);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zf);
		glEnd();
		if (isApresentaNormais) {
			desenhaNormal(xi, yi, zi, cross, red_plastic);
			desenhaNormal(xf, yi, zi, cross, red_plastic);
			desenhaNormal(xf, yf, zf, cross, red_plastic);
			desenhaNormal(xi, yi, zf, cross, red_plastic);
		}
		break;
	case ESTE_OESTE:
		v1[2] = zf - zi;
		v2[2] = 0;
		CrossProduct(v1, v2, cross);
		//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
		length = VectorNormalize(cross);
		//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);
		materials.material(red_plastic);
		glBegin(GL_QUADS);
		glNormal3dv(cross);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zf);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zi);
		glEnd();
		if (isApresentaNormais) {
			desenhaNormal(xi, yi, zi, cross, red_plastic);
			desenhaNormal(xf, yi, zf, cross, red_plastic);
			desenhaNormal(xf, yf, zf, cross, red_plastic);
			desenhaNormal(xi, yi, zi, cross, red_plastic);
		}
		break;
	default:
		cross[0] = 0;
		cross[1] = 0;
		cross[2] = 1;
		materials.material(azul);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glVertex3f(xi, yi, zi);
		glVertex3f(xf, yi, zf);
		glVertex3f(xf, yf, zf);
		glVertex3f(xi, yf, zi);
		glEnd();
		if (isApresentaNormais) {
			desenhaNormal(xi, yi, zi, cross, azul);
			desenhaNormal(xf, yi, zf, cross, azul);
			desenhaNormal(xf, yf, zf, cross, azul);
			desenhaNormal(xi, yi, zi, cross, azul);
		}
		break;
	}
}


void DesenhaLabirinto:: desenhaParede(GLfloat xi, GLfloat yi, GLfloat zi, GLfloat xf, GLfloat yf, GLfloat zf, GLboolean isApresentaNormais) {
	Materials materials = Materials();
	GLdouble v1[3], v2[3], cross[3];
	GLdouble length;
	v1[0] = xf - xi;
	v1[1] = yf - yi;
	v1[2] = 0;
	v2[0] = 0;
	v2[1] = 0;
	v2[2] = 1;
	CrossProduct(v1, v2, cross);
	//printf("cross x=%lf y=%lf z=%lf",cross[0],cross[1],cross[2]);
	length = VectorNormalize(cross);
	//printf("Normal x=%lf y=%lf z=%lf length=%lf\n",cross[0],cross[1],cross[2]);

	materials.material(emerald);
	glBegin(GL_QUADS);
	glNormal3dv(cross);
	glVertex3f(xi, yi, zi);
	glVertex3f(xf, yf, zf + 0);
	glVertex3f(xf, yf, zf + 1);
	glVertex3f(xi, yi, zi + 1);
	glEnd();

	if (isApresentaNormais) {
		desenhaNormal(xi, yi, zi, cross, emerald);
		desenhaNormal(xf, yf, zf, cross, emerald);
		desenhaNormal(xf, yf, zf + 1, cross, emerald);
		desenhaNormal(xi, yi, zi + 1, cross, emerald);
	}
}


void DesenhaLabirinto:: desenhaNo(int no, GLboolean isApresentaNormais) {
	GLboolean norte, sul, este, oeste;
	GLfloat larguraNorte, larguraSul, larguraEste, larguraOeste;
	Arco arco = arcos[0];
	No *noi = &nos[no], *nof;
	norte = sul = este = oeste = GL_TRUE;

	desenhaChao(nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, PLANO, isApresentaNormais);
	for (int i = 0; i<numArcos; arco = arcos[++i]) {
		if (arco.noi == no)
			nof = &nos[arco.nof];
		else
			if (arco.nof == no)
				nof = &nos[arco.noi];
			else
				continue;
		if (noi->x == nof->x)
			if (noi->y<nof->y) {
				norte = GL_FALSE;
				larguraNorte = arco.largura;
			}
			else {
				sul = GL_FALSE;
				larguraSul = arco.largura;
			}
		else
			if (noi->y == nof->y)
				if (noi->x<nof->x) {
					oeste = GL_FALSE;
					larguraOeste = arco.largura;
				}
				else {
					este = GL_FALSE;
					larguraEste = arco.largura;
				}
			else
			//	cout << "Arco dioagonal: " << arco.noi << " " << arco.nof << endl;
		if (norte && sul && este && oeste)
			return;
	}
	if (norte)
		desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, isApresentaNormais);
	else
		if (larguraNorte < noi->largura) {
			desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*larguraNorte, nos[no].y + 0.5*noi->largura, nos[no].z, isApresentaNormais);
			desenhaParede(nos[no].x + 0.5*larguraNorte, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, isApresentaNormais);
		}
	if (sul)
		desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, isApresentaNormais);
	else
		if (larguraSul < noi->largura) {
			desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*larguraSul, nos[no].y - 0.5*noi->largura, nos[no].z, isApresentaNormais);
			desenhaParede(nos[no].x - 0.5*larguraSul, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, isApresentaNormais);
		}
	if (este)
		desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, isApresentaNormais);
	else
		if (larguraEste < noi->largura) {
			desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y - 0.5*larguraEste, nos[no].z, isApresentaNormais);
			desenhaParede(nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*larguraEste, nos[no].z, nos[no].x - 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, isApresentaNormais);
		}
	if (oeste)
		desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, isApresentaNormais);
	else
		if (larguraOeste < noi->largura) {
			desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*noi->largura, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y + 0.5*larguraOeste, nos[no].z, isApresentaNormais);
			desenhaParede(nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*larguraOeste, nos[no].z, nos[no].x + 0.5*noi->largura, nos[no].y - 0.5*noi->largura, nos[no].z, isApresentaNormais);
		}
}



void DesenhaLabirinto:: desenhaNormal(GLdouble x, GLdouble y, GLdouble z, GLdouble normal[], tipo_material mat) {

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



void DesenhaLabirinto::CrossProduct(GLdouble v1[], GLdouble v2[], GLdouble cross[]) {
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

GLdouble DesenhaLabirinto:: VectorNormalize(GLdouble v[])
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

