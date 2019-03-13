#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/geometric.hpp>
#include "ballObject.h"

void setNormalCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 192.0, 0.0, 108.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void setBallCamera(glm::vec2 pos, float radius) {
	const float width = 192.0 * 3 / 4;
	const float height = 108.0 * 3 / 4;
	pos.x += radius;
	pos.y += radius;
	float x, y;

	if (pos.x < width / 2)
		x = 0;
	else if (pos.x > 192.0 - width / 2)
		x = 192.0 - width;
	else
		x = pos.x - width / 2;

	if (pos.y < height / 2)
		y = 0;
	else if (pos.y > 108.0 - height / 2)
		y = 108.0 - height;
	else
		y = pos.y - height / 2;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-x, -y, 0.0);
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int len(int n) {
	unsigned int num = 0;

	do {
		++num;
		n /= 10;
	} while (n);

	return num;
}

void renderScore(int score1, int score2) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0, 0, 0);

	char buffer[10];
	int len1 = len(score1);
	int len2 = len(score2);

	_itoa_s(score1, buffer, 10);
	glRasterPos2i(width / 10 - 9 * len1 / 2, height * 9 / 10 - 7);
	glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char *>(buffer));

	_itoa_s(score2, buffer, 10);
	glRasterPos2i(width * 9 / 10 - 9 * len2 / 2, height * 9 / 10 - 7);
	glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char *>(buffer));
}