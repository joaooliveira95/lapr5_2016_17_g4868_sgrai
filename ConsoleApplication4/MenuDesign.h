#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "TextureLoader.h"
#include "OverlaysDesign.h"
#include <GL/glut.h>
typedef struct {
	GLboolean   menuActivo;
	GLint index;
}Menu;

class MenuDesign
{
	glTexture menus[4];
	
public:
	MenuDesign();
	~MenuDesign();
	void drawMenu(GLboolean estado, GLint menu);
	void drawCharacterSelection(GLboolean estado, GLint menu, int n_player);
};

