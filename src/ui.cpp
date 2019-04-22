#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "ui.h"

// modified code from https://forums.khronos.org/showthread.php/19787
void drawEllipse(float x, float y, float xradius, float yradius, int startDegree, int endDegree)
{
	glBegin(GL_POLYGON);
	for (int i = startDegree; i <= endDegree; i++)
	{
		float degInRad = i * DEG2RAD;
		glVertex2f(x + xradius + cos(degInRad)*xradius, y + yradius + sin(degInRad)*yradius);
	}
	glEnd();
}

// modified code from https://forums.khronos.org/showthread.php/19787
void drawEllipseStroke(float x, float y, float xradius, float yradius, int startDegree, int endDegree)
{
	glBegin(GL_LINE_STRIP);
	for (int i = startDegree; i <= endDegree; i++)
	{
		float degInRad = i * DEG2RAD;
		glVertex2f(x + xradius + cos(degInRad)*xradius, y + yradius + sin(degInRad)*yradius);
	}
	glEnd();
}

void set2DCamera() {
	glShadeModel(GL_FLAT);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 192.0, 0.0, 108.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int bitmapStringLength(void* font, const char* text) {
	int length = 0;
	int i = 0;

	do {
		length += glutBitmapWidth(font, text[i]);
		i++;
	} while (text[i] != '\0');

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

	glRasterPos2i((int)(width * x - length / 2), (int)(height * y - 9));
	glutBitmapString(font, reinterpret_cast<const unsigned char *>(text));
}

void renderScore(int score1, int score2) {
	char buffer[10];
	glColor3f(0.0, 0.0, 0.0);
	_itoa_s(score1, buffer, 10);
	renderText(0.1f, 0.9f, buffer);
	_itoa_s(score2, buffer, 10);
	renderText(0.9f, 0.9f, buffer);
}

void renderReady(int delayTime) {
	char buffer[10];
	_itoa_s(delayTime / 1000 + 1, buffer, 10);
	glColor3f(1.0, 0.0, 0.0);
	renderText(0.5f, 0.9f, buffer);
}

void renderGo() {
	glColor3f(1.0, 0.0, 0.0);
	renderText(0.5f, 0.9f, "GO!");
}

void renderScoreText() {
	glColor3f(1.0, 0.0, 0.0);
	renderText(0.5f, 0.9f, "SCORE!");
}

void renderCameraText(bool ballCameraMode) {
	glColor3f(0.4f, 0.4f, 0.4f);

	if (ballCameraMode)
		renderText(0.5f, 0.9f, "Ball Camera (Spacebar to change)");
	else
		renderText(0.5f, 0.9f, "Normal Camera (Spacebar to change)");
}

void renderMenuPikachuHalf() {
	glColor3f(0.0, 0.0, 0.0);
	drawEllipse(10, 5, 8.0, 8.0);
	glColor3f(1.0, 1.0, 1.0);
	drawEllipse(11.5, 10.5, 4.0, 4.0);
	glColor3f(1.0, 0.0, 0.0);
	drawEllipse(16.5, -20.0, 7.0, 7.0);
	glColor3f(0.0, 0.0, 0.0);
	drawEllipseStroke(0.0, -10.0, 4.0, 2.0, 180, 370);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
		glVertex3f(2.0, 1.0, 0.0);
	}
	glEnd();
}

void renderMenu(bool is2player) {
	glColor3f(1.0f, 1.0f, 0.0f);
	glRectf(0.0f, 0.0f, 192.0f, 108.0f);

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(192.0f * 0.25f, 108.0f * 0.5f, 0.0f);

	renderMenuPikachuHalf();
	glScalef(-1.0f, 1.0f, 0.0f);
	renderMenuPikachuHalf();
	glScalef(-1.0f, 1.0f, 0.0f);

	glTranslatef(-192.0f * 0.25f, -108.0f * 0.5f, 0.0f);

	glColor3f(0.8f, 0.0f, 0.0f);
	renderText(0.75f, 0.65f, "PIKACHU VOLLEYBALL");

	if (is2player)
		glColor3f(0.4f, 0.4f, 0.4f);
	else
		glColor3f(0.0f, 0.0f, 0.0f);
	renderText(0.75f, 0.45f, "Single Play");

	if (is2player)
		glColor3f(0.0f, 0.0f, 0.0f);
	else
		glColor3f(0.4f, 0.4f, 0.4f);
	renderText(0.75f, 0.35f, "Multi Play");
}

void renderWinText(bool is2player, int score1, int score2) {
	glColor3f(1.0f, 0.0f, 0.0f);

	if (is2player) {
		if (score1 > score2)
			renderText(0.5f, 0.9f, "PLAYER 1 WIN!");
		else
			renderText(0.5f, 0.9f, "PLAYER 2 WIN!");
	}
	else {
		if (score1 > score2)
			renderText(0.5f, 0.9f, "YOU WIN!");
		else
			renderText(0.5f, 0.9f, "YOU LOSE ;(");
	}

	glColor3f(0.4f, 0.4f, 0.4f);
	renderText(0.5f, 0.8f, "(Press Enter)");
}