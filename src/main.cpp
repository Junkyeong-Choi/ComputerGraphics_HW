#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}


void init() {

}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Hello guys");
	glutDisplayFunc(renderScene);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glewInit();
	glutMainLoop();
}