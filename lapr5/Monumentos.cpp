#include "Monumentos.h"



Monumentos::Monumentos()
{
}


Monumentos::~Monumentos()
{
}


void Monumentos:: desenhaClerigos(Model_3DS clerigos) {
	glPushMatrix();
	glScalef(0.005, 0.005, 0.005);
	glRotatef(0, 1, 0, 0);
	clerigos.Draw();
	glPopMatrix();
}

void Monumentos::desenhaCasaMusica(Model_3DS musica) {
	glPushMatrix();
	glScalef(0.005, 0.005, 0.005);
	glTranslatef(15, 0, 0);
	glRotatef(0, 1, 0, 0);
	musica.Draw();
	glPopMatrix();
}