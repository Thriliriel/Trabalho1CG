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
	glClear(GL_COLOR_BUFFER_BIT);

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
				glBegin(GL_QUADS);

				glColor3f(0, 0, 0);

				glVertex2f(i, j);

				glColor3f(1, 1, 1);

				glVertex2f(i, j + 1);

				glColor3f(0, 0, 0);

				glVertex2f(i + 1, j + 1);

				glColor3f(1, 1, 1);

				glVertex2f(i + 1, j);

				glEnd();
			}

			if (indexControlI < scale-1) {
				indexControlI++;
			}
			else {
				indexControlI = 0;
				indexI++;
			}
		}
		
		if (indexControlJ < scale-1) {
			indexControlJ++;
		}
		else {
			indexControlJ = 0;
			indexJ++;
		}
	}

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
			changeCamera(10, 0);
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
	switch (a_keys)
	{
	case GLUT_KEY_UP:       // Se pressionar UP
		glutFullScreen(); // Vai para Full Screen
		break;
	case GLUT_KEY_DOWN:     // Se pressionar UP
							// Reposiciona a janela
		glutPositionWindow(50, 50);
		glutReshapeWindow(700, 500);
		break;
	default:
		break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int  main(int argc, char** argv)
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

/*
// TeaPot3D.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização 
// de objetos 3D.
// Este código está baseado nos exemplos disponíveis no livro 
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.


GLfloat angle, fAspect;

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 1.0f);

	// Desenha o teapot com a cor corrente (wire-frame)
	glutWireTeapot(50.0f);

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	angle = 45;
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	// Especifica posição do observador e do alvo
	gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(angle, fAspect, 0.5, 500);

	PosicionaObservador();
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (angle >= 10) angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (angle <= 130) angle += 5;
		}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Programa Principal
int main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(350, 300);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	Inicializa();
	glutMainLoop();
}*/