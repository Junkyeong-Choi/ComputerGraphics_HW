#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"
#include <iostream>

using namespace std;

const int width = 768;
const int height = 432;
bool isFullScreen = false;

//const int width = 1920;
//const int height = 1080;
//bool isFullScreen = true;

Game volleyball;
int lastFrame;

void display() {
	// Update lastFrame to prevent time delta being so large that makes ball go beyond the boundary
	lastFrame = glutGet(GLUT_ELAPSED_TIME);
	volleyball.render();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void close() {
	volleyball.exit();
}

void keyboard(unsigned char key, int x, int y) {
	volleyball.handleInput(key);
}

void keyboardUp(unsigned char key, int x, int y) {
	volleyball.handleInputUp(key);
}

void specialKeyboard(int key, int x, int y) {
	volleyball.handleSpecialInput(key);
}

void specialKeyboardUp(int key, int x, int y) {
	volleyball.handleSpecialInputUp(key);
}

// Architecture inspired by https://learnopengl.com/In-Practice/2D-Game/Breakout
int main(int argc, char* argv[]) {
	volleyball.init(argc, argv, width, height, isFullScreen);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKeyboard);
	glutSpecialUpFunc(specialKeyboardUp);

	int delta;
	lastFrame = glutGet(GLUT_ELAPSED_TIME);

	while (!volleyball.isExiting()) {
		glutMainLoopEvent();
		int currentFrame = glutGet(GLUT_ELAPSED_TIME);
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;
		volleyball.update(delta);
		volleyball.render();
	}

	return 0;
}