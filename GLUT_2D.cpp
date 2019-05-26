#include "pch.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "spline.h"
#include "coordsystem.h"

#pragma region GlobalVariables
//GLint Width = 1024, Height = 512;
GLint Width = 1004, Height = 570;

Spline spline;
CoordSystem CS;
#pragma endregion

void renderScene(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CS.drawGrid();
	spline.draw();
	
	glFinish();
    glutSwapBuffers();
}

/* Функция изменения размеров окна */
void Reshape(GLint w, GLint h){
	Width = w;    Height = h;
	// set new Dispaly size in Coordinate System
	CS.resizeDisplay(w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

#pragma region EventHandlers
/* Функция обработки сообщений от клавиатуры */
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':  CS.shift(0, 1); break;
	case 's':  CS.shift(0, -1); break;
	case 'a':  CS.shift(-1, 0); break;
	case 'd': CS.shift(1, 0); break;

	case 'q': CS.scale(0.5); break;
	case 'e': CS.scale(2); break;
	}
	glutPostRedisplay();
}

/* Функция обработки сообщения от мыши */
void Mouse(int button, int state, int x, int y) {
	/* клавиша была нажата, но не отпущена */
	if (state != GLUT_DOWN) return;

	if (button == GLUT_LEFT_BUTTON) {
		// need !transform coordinates
		Point displ = Point(x, y);
		Point scene = CS.getSceneCoordinate(displ);

		spline.addPoint(scene.x, scene.y);
	}
	glutPostRedisplay();
}
#pragma endregion

int main(int argc, char **argv) {
	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("2D application");
	
	// initialization of coord system
	CS = CoordSystem(Width, Height);
	
	// регистрация обратных вызовов
	glutDisplayFunc(renderScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);

	// Основной цикл GLUT
	glutMainLoop();
	return 1;
}