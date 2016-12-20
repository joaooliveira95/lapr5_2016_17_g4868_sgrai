#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <AL/alut.h>
#include <GL/glut.h>

class OverlaysDesign
{
public:
	OverlaysDesign();
	~OverlaysDesign();	
	void bitmapString(char *str, double x, double y);
	void bitmapCenterString(char *str, double x, double y);
	void desenhaKm(float x);
	void desenhaTempo(int x);
};

