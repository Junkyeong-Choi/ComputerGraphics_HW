#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "ui.h"
#include "settings.h"

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
	glUseProgram(0);
	glShadeModel(GL_FLAT);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, MAP_SIZE.x, 0.0, MAP_SIZE.y);

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

void renderMenuPikachuHalf() {
	glColor3f(0.0f, 0.0f, 0.0f);
	drawEllipse(10, 5, 8.0, 8.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawEllipse(11.5, 10.5, 4.0, 4.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawEllipse(16.5, -20.0, 7.0, 7.0);
	glColor3f(0.0f, 0.0f, 0.0f);
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
	glRectf(0.0f, 0.0f, MAP_SIZE.x, MAP_SIZE.y);

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(MAP_SIZE.x * 0.25f, MAP_SIZE.y * 0.5f, 0.0f);

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