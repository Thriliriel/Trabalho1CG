// TrabalhoOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include "gl\glut.h"
#include <time.h>
#include <iostream>

using namespace std;

//largura/altura do mapa
const int MAP_SIZE = 10;

//escala do mapa (default 1)
int scale = 1;

int Fps = 0;
float LastTickCount = 0.0f;
float CurrentTickCount;
char FrameRate[50] = "";

float movement = 0.1;
double move_y = 0;
double move_x = 0;
double move_z = 0;

//array de disposição do mapa
//0 = cubinhos
//1 = caminho livre
int map[MAP_SIZE*MAP_SIZE] = {
	0,0,0,1,0,0,1,0,0,0,
	0,0,0,1,0,0,1,0,0,0,
	0,0,0,1,0,0,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,
	0,0,0,1,0,0,1,0,0,0,
	0,0,0,1,0,0,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,
	0,0,0,1,0,0,1,0,0,0,
	0,0,0,1,0,0,1,0,0,0,
	0,0,0,1,0,0,1,0,0,0
};

// **********************************************************************
//  void init(void)
//  Inicializa os parâmetros globais de OpenGL
//
// **********************************************************************
void init(void)
{
	// Define a cor do fundo da tela (BRANCO)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Especifica posição do observador e do alvo
	//gluLookAt(0, 0, 10, 0, 0, 10, 0, 0, 0);
}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape(int w, int h)
{
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Define a área a ser ocupada pela área OpenGL dentro da Janela
	glViewport(0, 0, w, h);

	// Define os limites lógicos da área OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glOrtho(0, MAP_SIZE, 0, MAP_SIZE, 0, 1);
	glOrtho(-1,      // left
		MAP_SIZE * scale + 1,  // right
		MAP_SIZE * scale + 1, // bottom
		-1,      // top
		0,      // zNear
		1       // zFar
	);
}

void changeCamera(int x, int y) {
	// Especifica posição do observador e do alvo
	gluLookAt(0, 0, 0, 0, 0, 0, 0, 0, 0);

	glutPostRedisplay();
}

// **********************************************************************
//  void display( void )
//
// **********************************************************************
void display(void)
{

	// Limpa a tela coma cor de fundo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Define os limites lógicos da área OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glOrtho(-1, 11, -1, 11, 0, 1);
	glOrtho(-1,      // left
		MAP_SIZE * scale + 1,  // right
		MAP_SIZE * scale + 1, // bottom
		-1,      // top
		0,      // zNear
		1       // zFar
	);

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenha os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glLineWidth(3);

	int index = 0;
	int indexJ = 0;
	int indexI = 0;
	int indexControlJ = 0;
	int indexControlI = 0;

	// move quando o usuário usa as teclas
	/*glTranslatef(move_x, 0.0, 0.0);
	glTranslatef(0, move_y, 0.0);
	glTranslatef(0, 0, move_z);*/

	//glRotatef(rotate_x, 1.0, 0.0, 0.0);
	//glRotatef(rotate_y, 0.0, 1.0, 0.0);

	//With each triplet of values, you specify: 
	//1) where are you, 
	//2) which direction are you looking, and 
	//3) which direction is up (all in world space). 
	gluLookAt(move_x, move_y, 0, move_x, move_y, -1, 0, 1, 0);
	//glTranslatef(0, 0, move_z);

	for (int j = 0; j < MAP_SIZE*scale; j++) {
		indexI = 0;

		if (indexControlJ == 0) {
			//se map nessa posição for 0, desenha
			index = (indexJ * MAP_SIZE) + indexI;
		}

		for (int i = 0; i < MAP_SIZE*scale; i++) {
			if (indexControlI == 0) {
				//se map nessa posição for 0, desenha
				index = (indexJ * MAP_SIZE) + indexI;
			}

			if (map[index] == 0) {
				//Lado preto/branco - FRENTE
				glBegin(GL_POLYGON);
				glColor3f(0, 0, 0);
				glVertex3f(i, j, 0);
				glColor3f(1, 1, 1);
				glVertex3f(i, j + 1, 0);
				glColor3f(0, 0, 0);
				glVertex3f(i + 1, j + 1, 0);
				glColor3f(1, 1, 1);
				glVertex3f(i + 1, j, 0);
				glEnd();

				//outros lados - preto
				glColor3f(0, 0, 0);
				// TRASEIRA
				glBegin(GL_POLYGON);
				//glColor3f(1.0, 1.0, 1.0);
				glVertex3f(i, j, -1);
				glVertex3f(i, j + 1, -1);
				glVertex3f(i + 1, j + 1, -1);
				glVertex3f(i + 1, j, -1);
				glEnd();

				// DIREITA
				glBegin(GL_POLYGON);
				//glColor3f(1.0, 0.0, 1.0);
				glVertex3f(i + 1, j, 0);
				glVertex3f(i + 1, j + 1, 0);
				glVertex3f(i + 1, j + 1, -1);
				glVertex3f(i + 1, j, -1);
				glEnd();

				// ESQUERDA
				glBegin(GL_POLYGON);
				//glColor3f(0.0, 1.0, 0.0);
				glVertex3f(i, j, 0);
				glVertex3f(i, j + 1, 0);
				glVertex3f(i, j + 1, -1);
				glVertex3f(i, j, -1);
				glEnd();

				// TOPO
				glBegin(GL_POLYGON);
				//glColor3f(0.0, 0.0, 1.0);
				glVertex3f(i, j + 1, 0);
				glVertex3f(i, j + 1, -1);
				glVertex3f(i + 1, j + 1, -1);
				glVertex3f(i + 1, j + 1, 0);
				glEnd();

				// BASE
				glBegin(GL_POLYGON);
				//glColor3f(1.0, 0.0, 0.0);
				glVertex3f(i, j, 0);
				glVertex3f(i, j, -1);
				glVertex3f(i + 1, j, -1);
				glVertex3f(i + 1, j, 0);
				glEnd();
			}

			if (indexControlI < scale - 1) {
				indexControlI++;
			}
			else {
				indexControlI = 0;
				indexI++;
			}
		}

		if (indexControlJ < scale - 1) {
			indexControlJ++;
		}
		else {
			indexControlJ = 0;
			indexJ++;
		}
	}

	glFlush();
	glutSwapBuffers();

	/*CurrentTickCount = clock() * 0.001f;
	Fps++;

	if ((CurrentTickCount - LastTickCount) > 1.0f)
	{
		LastTickCount = CurrentTickCount;
		std::cout << (FrameRate, "[ FPS: %d ]", Fps);
		Fps = 0;
	}*/
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
		case 27:        // Termina o programa qdo
			exit(0);   // a tecla ESC for pressionada
			break;
		case 119: //w
			move_z += movement;
			//  Requisitar atualização do display
			glutPostRedisplay();
			break;
		case 115: //s
			move_z -= movement;
			//  Requisitar atualização do display
			glutPostRedisplay();
			break;
		default:
			break;
	}
}


// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys(int a_keys, int x, int y)
{
	//  Seta direita - aumenta rotação em 5 graus
	if (a_keys == GLUT_KEY_RIGHT)
		move_x += movement;

	//  Seta para esquerda - diminui a rotação por 5 graus
	else if (a_keys == GLUT_KEY_LEFT)
		move_x -= movement;

	else if (a_keys == GLUT_KEY_UP)
		move_y -= movement;

	else if (a_keys == GLUT_KEY_DOWN)
		move_y += movement;

	//  Requisitar atualização do display
	glutPostRedisplay();
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(0, 0);

	// Define o tamanho inicial da janela grafica do programa
	glutInitWindowSize(650, 500);

	// Cria a janela na tela, definindo o nome da
	// que aparecera na barra de título da janela.
	glutCreateWindow("Primeiro Programa em OpenGL");

	// executa algumas inicializações
	init();

	// Habilite o teste de profundidade do Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Define que o tratador de evento para
	// o redesenho da tela. A funcao "display"
	// será chamada automaticamente quando
	// for necessário redesenhar a janela
	glutDisplayFunc(display);

	// Define que o tratador de evento para
	// o redimensionamento da janela. A funcao "reshape"
	// será chamada automaticamente quando
	// o usuário alterar o tamanho da janela
	glutReshapeFunc(reshape);

	// Define que o tratador de evento para
	// as teclas. A funcao "keyboard"
	// será chamada automaticamente sempre
	// o usuário pressionar uma tecla comum
	glutKeyboardFunc(keyboard);

	// Define que o tratador de evento para
	// as teclas especiais(F1, F2,... ALT-A,
	// ALT-B, Teclas de Seta, ...).
	// A funcao "arrow_keys" será chamada
	// automaticamente sempre o usuário
	// pressionar uma tecla especial
	glutSpecialFunc(arrow_keys);

	// inicia o tratamento dos eventos
	glutMainLoop();

	return 0;
}