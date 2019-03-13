#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/geometric.hpp>
#include "ballObject.h"
#include <iostream>

using namespace std;

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

int bitmapStringLength(void* font, const char* text) {
	int length = 0;
	int i = 0;

	do {
		length += glutBitmapWidth(font, text[i]);
		i++;
	} while (text[i] != '\0' && i < 10);

	return length;
}

void renderText(float x, float y, const char* text) {
	void* font = GLUT_BITMAP_HELVETICA_18;
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int length = bitmapStringLength(font, text);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0, 0, 0);

	glRasterPos2i(width * x - length / 2, height * y - 9);
	glutBitmapString(font, reinterpret_cast<const unsigned char *>(text));
}

void renderScore(int score1, int score2) {
	char buffer[10];
	_itoa_s(score1, buffer, 10);
	renderText(0.1, 0.9, buffer);
	_itoa_s(score2, buffer, 10);
	renderText(0.9, 0.9, buffer);
}

void renderReady(int delayTime) {
	char buffer[10];
	_itoa_s(delayTime / 1000 + 1, buffer, 10);
	renderText(0.5, 0.9, buffer);
}

void renderGo() {
	renderText(0.5, 0.9, "GO!");
}

void renderScoreText() {
	renderText(0.5, 0.9, "SCORE!");
}