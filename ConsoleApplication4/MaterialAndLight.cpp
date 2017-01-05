#include "MaterialAndLight.h"



MaterialAndLight::MaterialAndLight()
{
}


MaterialAndLight::~MaterialAndLight()
{
}


void MaterialAndLight:: setLight() {
	GLfloat light_pos[4] = { 0.0, 0.0, 20.0, 0.0 };
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// ligar iluminação
	glEnable(GL_LIGHTING);
	// ligar e definir fonte de light 0
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void MaterialAndLight:: setMaterial() {
	GLfloat mat_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_shininess = 104;
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// definir os outros parâmetros estaticamente
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

void MaterialAndLight::setLightCharacter(GLfloat x, GLfloat y, GLfloat z) {
	GLfloat luz_dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat luz_spec[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat luz_pos[4] = { 0.0, 0.0, 20.0, 0.0 };
	GLfloat luz_dir[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float luzpos[] = { x, y , z+10, 1.0 };
	float luzdir[] = { 0.0, 0.0, -1.0 };	

	glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, luz_spec);
	glLightfv(GL_LIGHT1, GL_POSITION, luz_pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, luz_dir);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0); // qualquer valor entre 0.0 e 90.0 graus

}
