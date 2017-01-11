#pragma once
#include <stdio.h>
#include <string.h>
#include <String>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include "TextureLoader.h"

using namespace std;

class OverlaysDesign
{
public:
	OverlaysDesign();
	~OverlaysDesign();	
	void bitmapString(char *str, double x, double y);
	void bitmapCenterString(char *str, double x, double y);
	void desenhaKm(float x);
	void desenhaTempo(int seg,int min);
	void infoOverlay(const char* nome, const char* descricao, const char* abertura,const char* fecho, glTexture textura);
	void desenhaNomeRua(string rua);
};

