#include <math.h>
#include <stdlib.h>     
#include <GL\glut.h>
#include <iostream>
#include <GL/glaux.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "Model_3DS.h"

#pragma once
class Monumentos
{
public:
	Monumentos();
	~Monumentos();
	void desenhaClerigos(Model_3DS clerigos);
	void desenhaCasaMusica(Model_3DS musica);
};

