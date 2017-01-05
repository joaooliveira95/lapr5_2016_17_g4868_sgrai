#pragma once
#include <GL/glut.h>
class MaterialAndLight
{
public:
	MaterialAndLight();
	~MaterialAndLight();
	void setMaterial();
	void setLight();
	void setLightCharacter(GLfloat x, GLfloat y, GLfloat z);
};

