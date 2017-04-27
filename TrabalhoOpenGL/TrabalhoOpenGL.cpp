// Iluminacao.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização 
// de objetos 3D com a inserção de uma fonte de luz.
// Este código está baseado nos exemplos disponíveis no livro 
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.

#include "stdafx.h"
#include <gl/glut.h>
//#include <Windows.h>
#include "csvReader.h"
#include <cstdlib>

GLfloat angle = 45, fAspect;
//largura do cubo eh de 10. Logo, o tamanho do cenário vai ser a quantidade de espaços x10.
//ex: cenário padrão 150x100 = tamanho 1500x1000
std::vector<std::vector<int>> myMap;
//escala do mapa (default 1)
int scale = 1;
int MAP_SIZE;
int MAP_WIDTH;
//distancia da camera
int camFarAway = 100;
//tamanho de cada célula
int cellSize = camFarAway * 2;
//int cellSize = 200;
//mapa para subdivisao de espaço
std::vector<std::vector<std::vector<int>>> mySubMap;

static float fpsTimer = 0.0f;
static float before = 0.0f;
static float now = 0.0f;

// actual vector representing the camera's direction
Vector3 selfPosition(60, 0, 3);
Vector3 camPosition(60, 30, 2.999f);

void EspecificaParametrosVisualizacao(void);

//otimizações
bool detectarColisoes = false;
bool subdividirEspaco = false;

int qntCubos = 0;
int cubeWidth(10);
int cubeLength(10);
int bonusCellI = 0;
int bonusCellJ = -cellSize / 10;

//distance between 2 points
float Distance(Vector3 start, Vector3 end)
{
	float result = sqrt((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y) + (end.z - start.z)*(end.z - start.z));

	return result;
}

float GetAngle(Vector3 point1, Vector3 point2, Vector3 origin)
{
	float angle1 = 0, angle2 = 0;

	//angle1 = atan2(EndPointB()->y - EndPointA()->y, EndPointB()->x - EndPointA()->x);
	angle1 = atan2(point1.y - origin.y, point1.x - origin.x);

	angle2 = atan2(point2.y - origin.y, point2.x - origin.x);

	float result = angle2 - angle1;

	//Multiply all values by 180/PI
	result *= (180.0f / 3.14159265359f);

	return result;
}

void fps(int value)
{
	
	now = glutGet(GLUT_ELAPSED_TIME);

	++fpsTimer;
	//std::cout << "Now: " << now << " - FPS: " << fpsTimer << "\n";
	if (now - before > 1000.0f)
	{
		before = now;
		std::cout << "FPS:" << int(fpsTimer) << std::endl;
		fpsTimer = 0.0f;
		//glutPostRedisplay();
	}
	//fps
	glutTimerFunc(1, fps, value);
}

void DrawCube(double cubeCoord_x, double cubeCoord_y, double cubeCoord_z, double width, double length, double height)
{
	// Desenha um cubo
	glBegin(GL_QUADS);			// Face posterior
	glNormal3f(0.0, 0.0, 1.0);	// Normal da face
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z + height);
	glEnd();

	glBegin(GL_QUADS);			// Face frontal
	glNormal3f(0.0, 0.0, -1.0); 	// Normal da face
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z);
	glEnd();

	glBegin(GL_QUADS);			// Face lateral esquerda
	glNormal3f(-1.0, 0.0, 0.0); 	// Normal da face
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z + height);
	glEnd();

	glBegin(GL_QUADS);			// Face lateral direita
	glNormal3f(1.0, 0.0, 0.0);	// Normal da face
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z);
	glEnd();

	glBegin(GL_QUADS);			// Face superior
	glNormal3f(0.0, 1.0, 0.0);  	// Normal da face
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z);
	glVertex3f(cubeCoord_x, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z + height);
	glVertex3f(cubeCoord_x + width, cubeCoord_y + length, cubeCoord_z);
	glEnd();

	glBegin(GL_QUADS);			// Face inferior
	glNormal3f(0.0, -1.0, 0.0); 	// Normal da face
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z);
	glVertex3f(cubeCoord_x + width, cubeCoord_y, cubeCoord_z + height);
	glVertex3f(cubeCoord_x, cubeCoord_y, cubeCoord_z + height);
	glEnd();
}

void DesenhaCelula(int indCell) {
	//pega o valor em matriz
	int cellX = indCell / (MAP_SIZE / (cellSize / 10));
	int cellY = indCell % (MAP_SIZE / (cellSize / 10));

	//bonus cell
	bonusCellI = cellX * cellSize / 10;
	bonusCellJ = cellY * cellSize / 10;

	int h = 0;
	//desenha essa célula
	for (int i = 0; i < cellSize / 10; i++) {
		for (int j = 0; j < cellSize / 10; j++) {
			double b_color = 1.0f;
			double g_color = i % 2 / 2.0f;
			double r_color = j % 2 / 2.0f;
			glColor3f(r_color, g_color, b_color);

			//se detectar colisoes
			if (detectarColisoes) {
				//calcula o centro do cubo
				Vector3 centroCubo(((i * cubeWidth + i) + (bonusCellI * cubeWidth)) + cubeWidth / 2, 
					((j * cubeLength + j) + (bonusCellJ * cubeLength)) + cubeLength / 2, 0);

				//testa o angulo do cubo com o angulo da camera
				float angulo = GetAngle(camPosition, centroCubo, selfPosition);

				//std::cout << angulo << " .. ";

				//soh desenha se estiver dentro do angulo de perspectiva
				//maior que o -angulo de abertura e menor que o angulo de abertura
				if (angulo <= angle && angulo >= (angle - 2 * angle)) {
				//se estiver dentro do angulo, verifica se está dentro da distancia limite (camFarAway)
					if (Distance(selfPosition, centroCubo) <= camFarAway) {
						//desenha o cubo
						if (mySubMap[indCell][i][j] > 0) {
							qntCubos++;
							DrawCube((i * cubeWidth + i) + (bonusCellI * cubeWidth), (j * cubeLength + j) + (bonusCellJ * cubeLength), 0, cubeWidth, cubeLength, mySubMap[indCell][i][j]);
						}
					}
				}
			}//senao, soh desenha
			else {
				//desenha o cubo
				//@TODO: o mapa nao está ficando igual ao do sem Subdivisao (invertido, talvez...?)
				if (mySubMap[indCell][i][j] > 0) {
					qntCubos++;
					DrawCube((i * cubeWidth + i) + (bonusCellI * cubeWidth), (j * cubeLength + j) + (bonusCellJ * cubeLength), 0, cubeWidth, cubeLength, mySubMap[indCell][i][j]);
				}
			}
		}
	}
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	qntCubos = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//branco, para dar melhor contraste
	//glColor3f(1, 1, 1);

	int line(0);
	
	//espaço subdivido
	if (subdividirEspaco) {
		//a partir da posição da camera, encontrar a celula dela
		int posCamCellX = selfPosition.x / cellSize;
		int posCamCellY = selfPosition.y / cellSize;

		//std::cout << "CamCell: " << posCamCellX << "x" << posCamCellY << "\n";

		int qntCells = MAP_SIZE / (cellSize / 10);
		int indCamCell = (posCamCellX * qntCells) + posCamCellY;

		//inicia com um valor não válido, uma vez que pode estar em algum corner
		int vizinhaEsquerda = -1;
		int vizinhaDireita = -1;
		int vizinhaEsquerdaFrente = -1;
		int vizinhaFrente = -1;
		int vizinhaDireitaFrente = -1;

		//se existir para a esquerda, seta
		if (posCamCellX - 1 >= 0) {
			vizinhaEsquerda = ((posCamCellX - 1) * qntCells) + posCamCellY;
		}
		//se existir para a direita, seta
		if (posCamCellX + 1 < qntCells) {
			vizinhaDireita = ((posCamCellX + 1) * qntCells) + posCamCellY;
		}
		//se existir esquerda frente, seta
		if (posCamCellX - 1 >= 0 && posCamCellY + 1 < qntCells) {
			vizinhaEsquerdaFrente = ((posCamCellX - 1) * qntCells) + (posCamCellY + 1);
		}
		//se existir frente, seta
		if (posCamCellY + 1 < qntCells) {
			vizinhaFrente = (posCamCellX * qntCells) + (posCamCellY + 1);
		}
		//se existir direita frente, seta
		if (posCamCellX + 1 < qntCells && posCamCellY + 1 < qntCells) {
			vizinhaDireitaFrente = ((posCamCellX + 1) * qntCells) + (posCamCellY + 1);
		}

		//só precisa desenhar essa celula da cam e as vizinhas dos lados e da frente
		DesenhaCelula(indCamCell);

		//vizinhas
		if (vizinhaEsquerda > -1) {
			DesenhaCelula(vizinhaEsquerda);
		}
		if (vizinhaDireita > -1) {
			DesenhaCelula(vizinhaDireita);
		}
		if (vizinhaEsquerdaFrente > -1) {
			DesenhaCelula(vizinhaEsquerdaFrente);
		}
		if (vizinhaFrente > -1) {
			DesenhaCelula(vizinhaFrente);
		}
		if (vizinhaDireitaFrente > -1) {
			DesenhaCelula(vizinhaDireitaFrente);
		}

		/*for (int c = 0; c < mySubMap.size(); c++) {
			//bonus cell
			if (c > 0 && c % (MAP_SIZE / (cellSize / 10)) == 0) {
				bonusCellI += cellSize / 10;
				bonusCellJ = 0;
			}
			else {
				bonusCellJ += cellSize / 10;
			}

			for (int i = 0; i < cellSize / 10; i++) {
				for (int j = 0; j < cellSize / 10; j++) {
					double b_color = 1.0f;
					double g_color = i % 2 / 2.0f;
					double r_color = j % 2 / 2.0f;
					glColor3f(r_color, g_color, b_color);

					//se detectar colisoes
					if (detectarColisoes) {
						//calcula o centro do cubo
						/*Vector3 centroCubo(((i * cubeWidth + i)) + cubeWidth / 2, ((j * cubeLength + j)) + cubeLength / 2, 0);

						//testa o angulo do cubo com o angulo da camera
						float angulo = GetAngle(camPosition, centroCubo, selfPosition);

						//std::cout << angulo << " .. ";

						//soh desenha se estiver dentro do angulo de perspectiva
						//maior que o -angulo de abertura e menor que o angulo de abertura
						if (angulo <= angle && angulo >= (angle - 2 * angle)) {
							//se estiver dentro do angulo, verifica se está dentro da distancia limite (camFarAway)
							if (Distance(selfPosition, centroCubo) <= camFarAway) {
								//desenha o cubo
								if (mySubMap[c][i][j] > 0) {
									qntCubos++;
									DrawCube((i * cubeWidth + i) + (bonusCellI * cubeWidth), (j * cubeLength + j) + (bonusCellJ * cubeLength), 0, cubeWidth, cubeLength, mySubMap[c][i][j]);
								}
							}
						}*/
					/*}//senao, soh desenha
					else {
						//desenha o cubo
						//@TODO: o mapa nao está ficando igual ao do sem Subdivisao (invertido, talvez...?)
						if (mySubMap[c][i][j] > 0) {
							qntCubos++;
							DrawCube((i * cubeWidth + i) + (bonusCellI * cubeWidth), (j * cubeLength + j) + (bonusCellJ * cubeLength), 0, cubeWidth, cubeLength, mySubMap[c][i][j]);
						}
					}
				}
			}
		}*/
	}
	else {
		//laço com escala
		int indexJ = 0;
		int indexI = 0;
		int indexControlJ = 0;
		int indexControlI = 0;

		for (int i = 0; i < MAP_SIZE*scale; ++i)
		{
			indexI = 0;

			for (int j = 0; j < MAP_SIZE*scale; ++j)
			{
				double b_color = 1.0f;
				double g_color = i % 2 / 2.0f;
				double r_color = j % 2 / 2.0f;
				glColor3f(r_color, g_color, b_color);

				//se detectar colisoes
				if (detectarColisoes) {
					//calcula o centro do cubo
					Vector3 centroCubo(((i * cubeWidth + i)) + cubeWidth / 2, ((j * cubeLength + j)) + cubeLength / 2, 0);

					//testa o angulo do cubo com o angulo da camera
					float angulo = GetAngle(camPosition, centroCubo, selfPosition);

					//std::cout << angulo << " .. ";

					//soh desenha se estiver dentro do angulo de perspectiva
					//maior que o -angulo de abertura e menor que o angulo de abertura
					if (angulo <= angle && angulo >= (angle - 2 * angle)) {
						//se estiver dentro do angulo, verifica se está dentro da distancia limite (camFarAway)
						if (Distance(selfPosition, centroCubo) <= camFarAway) {
							//desenha o cubo
							if (myMap[indexI][indexJ] > 0) {
								qntCubos++;
								DrawCube(i * cubeWidth + i, j * cubeLength + j, 0, cubeWidth, cubeLength, myMap[indexI][indexJ]);
							}
						}
					}
				}//senao, soh desenha
				else {
					//desenha o cubo
					if (myMap[indexI][indexJ] > 0) {
						qntCubos++;
						DrawCube(i * cubeWidth + i, j * cubeLength + j, 0, cubeWidth, cubeLength, myMap[indexI][indexJ]);
					}
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
	}

	std::cout << "Qnt Cubos: " << qntCubos << "\n";

	//fps();
	//Sleep(0);
	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa(void)
{
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };		 // "cor" 
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };// "brilho" 
	GLfloat posicaoLuz[4] = { 0.0, 50.0, 50.0, 1.0 };

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	//if (angle > 180) angle = 180;
	EspecificaParametrosVisualizacao();
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle, fAspect, 0.4, camFarAway);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(selfPosition.x, selfPosition.y, selfPosition.z, camPosition.x, camPosition.y, camPosition.z, 0, 1, 0);

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
	//if (button == GLUT_LEFT_BUTTON)
	//	if (state == GLUT_DOWN) {  // Zoom-in
	//		if (angle >= 10) angle -= angleMovement;
	//	}
	//if (button == GLUT_RIGHT_BUTTON)
	//	if (state == GLUT_DOWN) {  // Zoom-out
	//		if (angle <= 170) angle += angleMovement;
	//	}
	if (button == GLUT_LEFT_BUTTON)
	{
		selfPosition.z += 5;
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		selfPosition.z -= 5;
		if (selfPosition.z <= 3)
		{
			selfPosition.z = 3;
		}
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:        // Termina o programa qdo
			exit(0);   // a tecla ESC for pressionada
			break;
		case 119: //w
			++selfPosition.y;
			++camPosition.y;
			break;
		case 115: //s
			--selfPosition.y;
			--camPosition.y;
			break;
		case 97: //a
			--selfPosition.x;
			--camPosition.x;
			break;
		case 100: //d
			++selfPosition.x;
			++camPosition.x;
			break;
		/*case 113: //q
			--camPosition_x;
			break;
		case 101: //e
			++camPosition_x;
			break;*/
		default:
			break;
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy)
{

	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT:
			--selfPosition.x;
			--camPosition.x;
			break;
		case GLUT_KEY_RIGHT:
			++selfPosition.x;
			++camPosition.x;
			break;
		case GLUT_KEY_UP:
			++selfPosition.y;
			++camPosition.y;
			break;
		case GLUT_KEY_DOWN:
			--selfPosition.y;
			--camPosition.y;
			break;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Programa Principal
int main(void)
{
	csvReader myCsvReader("input240x240.csv");
	myMap = myCsvReader.getAllLines();
	MAP_SIZE = myMap.size();
	MAP_WIDTH = myMap[0].size();

	//definir a estrutura subdividida
	if (subdividirEspaco) {
		//para cada celula
		std::vector<std::vector<int>> flagMap(cellSize/10);
		//inicializa
		for (int k = 0; k < flagMap.size(); k++) {
			for (int l = 0; l < cellSize / 10; l++) {
				flagMap[k].push_back(0);
			}
		}
		
		//quantidade de celulas
		int qntCells = MAP_SIZE / (cellSize / 10);
		qntCells *= qntCells;

		//preenche
		//bonus cell
		for (int c = 0; c < qntCells; c++) {
			//bonus cell
			if (c > 0 && c % (MAP_SIZE / (cellSize / 10)) == 0) {
				bonusCellI += cellSize / 10;
				bonusCellJ = 0;
			}
			else {
				bonusCellJ += cellSize / 10;
			}

			for (int i = 0; i < cellSize / 10; i++) {
				for (int j = 0; j < cellSize / 10; j++) {
					flagMap[i][j] = myMap[i + bonusCellI][j + bonusCellJ];
				}
			}

			mySubMap.push_back(flagMap);
		}
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 350);
	glutCreateWindow("CG - T1 - Paulo");
	//glutIdleFunc(Desenha);
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(2, fps, 0);
	Inicializa();
	glutMainLoop();
}
