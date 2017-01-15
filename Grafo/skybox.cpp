#include "skybox.h"

#include <stdio.h>  
#include <gl\gl.h>  
#include <gl\glaux.h>

#define GL_TEXTURE_CUBE_MAP_ARB             0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB  0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB  0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB  0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB  0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB  0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB  0x851A

bool SKYBOX::Initialize()
{
	// Teste ao suporte da extensão GL_ARB_texture_cube_map
	char* extensions = (char*) glGetString(GL_EXTENSIONS); 

	if(strstr(extensions, "GL_ARB_texture_cube_map") != NULL)
	{

		GLenum cube_map_target[6] = {           
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
		};

		// Carregamento das 6 imagens
		AUX_RGBImageRec * texture_image[6];
		texture_image[0] = LoadBMP( "Skybox/XN.bmp" );
		texture_image[1] = LoadBMP( "Skybox/XP.bmp" );
		texture_image[2] = LoadBMP( "Skybox/YN.bmp" );
		texture_image[3] = LoadBMP( "Skybox/YP.bmp" );
		texture_image[4] = LoadBMP( "Skybox/ZN.bmp" );
		texture_image[5] = LoadBMP( "Skybox/ZP.bmp" );

		// Geração de um CubeMap com texturas
		glGenTextures(1, &cube_map_texture_ID);

		// Configuração da textura
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cube_map_texture_ID);

		for (int i = 0; i < 6; i++)
		{
			if (texture_image[i])				
			{
				glTexImage2D(cube_map_target[i], 0, 3, texture_image[i]->sizeX, texture_image[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_image[i]->data);

				if (texture_image[i]->data)	
				{
					free(texture_image[i]->data);	
				}
				free(texture_image[i]);	
			}
		}

		// Configuração dos parâmetros da textura
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
	
		return true;
	} 

	return false;
}

void SKYBOX::Render( float camera_yaw, float camera_pitch )
{
	// Configuração dos estados OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB); 
	glDisable(GL_LIGHTING);

	// Desativação de escrita no DepthBuffer
	glDepthMask(GL_FALSE);

	// Renderização da SkyBox
	DrawSkyBox( camera_yaw, camera_pitch );

	// Reactivação da escrita para o DepthBuffer
	glDepthMask(GL_TRUE);

	// Reactivação dos estados OpenGL
	glDisable(GL_TEXTURE_CUBE_MAP_ARB); 
	glEnable(GL_LIGHTING);
}

void SKYBOX::Finalize()
{
	// Supressão da SkyBox
	glDeleteTextures( 1, &cube_map_texture_ID );
}

void SKYBOX::DrawSkyBox( float camera_yaw, float camera_pitch )
{
	// Tamanho do cubo
	float t = 1.0f;

	// Utilização da textura CubeMap
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cube_map_texture_ID);

	// Ajustamento da orientação
	glPushMatrix();
	glLoadIdentity();
	glRotatef( camera_pitch, 1.0f, 0.0f, 0.0f );
	glRotatef( camera_yaw, 0.0f, 1.0f, 0.0f );	
	

	// Renderização da geometria
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_STRIP);			// X Negativo		
		glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t); 	
		glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
		glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
		glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// X Positivo
		glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
		glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
		glTexCoord3f(t,t,-t); glVertex3f(t,t,-t); 
		glTexCoord3f(t,t,t); glVertex3f(t,t,t); 	
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Y Negativo	
		glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);
		glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
		glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
		glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Y Positivo		
		glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
		glTexCoord3f(t,t,-t); glVertex3f(t,t,-t); 
		glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
		glTexCoord3f(t,t,t); glVertex3f(t,t,t); 	
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Z Negativo		
		glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);
		glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
		glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
		glTexCoord3f(t,t,-t); glVertex3f(t,t,-t); 
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);			// Z Positivo	
		glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
		glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
		glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
		glTexCoord3f(t,t,t); glVertex3f(t,t,t); 	
	glEnd();

	// Reactivação da matriz ModelView
	glPopMatrix();
}

AUX_RGBImageRec * SKYBOX::LoadBMP(char *Filename)  // Carrega uma imagem bitmap
{
	FILE *File=NULL;                       // Descritor do ficheiro

	if (!Filename)    // Verifica se o nome do ficheiro não é nulo
	{
		return NULL;  // Se não, retorna NULL
	}

	File=fopen(Filename,"r");  // Verifica se o ficheiro existe

	if (File)                              // O ficheiro existe?
	{
		fclose(File);                      // Fecha o descritor
		return auxDIBImageLoad(Filename);  // Abre o bitmap e retorna um apontador
	}

	return NULL;  // Retorna NULL se o carregamento falhou
}