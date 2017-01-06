#include "MenuDesign.h"


MenuDesign::MenuDesign()
{
	TextureLoader tl = TextureLoader();

	tl.LoadTextureFromDisk("menuTipoUtilizador.jpg", &menus[0]);
	tl.LoadTextureFromDisk("menuop2.jpg", &menus[1]);
	tl.LoadTextureFromDisk("menukeys.jpg", &menus[2]);
	tl.LoadTextureFromDisk("menuop3.jpg", &menus[3]);

}

MenuDesign::~MenuDesign()
{
}

void MenuDesign::drawMenu(GLboolean estado, GLint menu){

	if (estado) {
		TextureLoader tl = TextureLoader();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, menus[menu].TextureID);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-400, 400, -300, 300);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glColor3f(2, 2, 2);
		glBegin(GL_POLYGON);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(400, 300);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-400, 300);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-400, -300);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(400, -300);

		glEnd();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);

		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);

	}

}



void MenuDesign::drawCharacterSelection(GLboolean estado, GLint menu, int n_player) {
	char str[255];

	float width = glutGet(GLUT_WINDOW_WIDTH) / 2;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	if (estado) {
		TextureLoader tl = TextureLoader();
		glTexture chao_texture;
		if (n_player == 1) {
			if (menu == 1)
				tl.LoadTextureFromDisk("char1_1.jpg", &chao_texture);
			else if (menu == 2)
				tl.LoadTextureFromDisk("char1_2.jpg", &chao_texture);
			else if (menu == 3)
				tl.LoadTextureFromDisk("char1_3.jpg", &chao_texture);
		}
		else if (n_player == 2) {
			if (menu == 1)
				tl.LoadTextureFromDisk("char2_1.jpg", &chao_texture);
			else if (menu == 2)
				tl.LoadTextureFromDisk("char2_2.jpg", &chao_texture);
			else if (menu == 3)
				tl.LoadTextureFromDisk("char2_3.jpg", &chao_texture);
		}
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, chao_texture.TextureID);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-400, 400, -300, 300);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glColor3f(2, 2, 2);

		glBegin(GL_POLYGON);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(400, 300);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-400, 300);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-400, -300);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(400, -300);

		glEnd();
		OverlaysDesign o = OverlaysDesign();
		glPushAttrib(GL_CURRENT_BIT);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);

		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);

	}

}
