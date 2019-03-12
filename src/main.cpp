#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <tuple>
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include "gameObjects.h"

Game game;


void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);
	
	game.gameInit();
}


void renderScene() {

}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(960, 580);
	glutCreateWindow(argv[0]);
	init();
}