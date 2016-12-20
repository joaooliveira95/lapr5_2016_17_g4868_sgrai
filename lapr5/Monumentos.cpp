#include "Monumentos.h"



Monumentos::Monumentos()
{
}


Monumentos::~Monumentos()
{
}


void Monumentos:: desenhaClerigos(Model_3DS clerigos) {
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	clerigos.Draw();
	glPopMatrix();
}

void Monumentos::desenhaCasaMusica(Model_3DS musica) {
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(15, 0, 0);
	glRotatef(90, 1, 0, 0);
	musica.Draw();
	glPopMatrix();
}