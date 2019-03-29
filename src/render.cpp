#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/geometric.hpp>
#include "ballObject.h"
#include "movableRectangleObject.h"
#include "game.h"
#include "render.h"

// modified code from https://forums.khronos.org/showthread.php/19787
void drawEllipse(float x, float y, float xradius, float yradius, int startDegree = 0, int endDegree = 360)
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
void drawEllipseStroke(float x, float y, float xradius, float yradius, int startDegree = 0, int endDegree = 360)
{
	glBegin(GL_LINE_STRIP);
	for (int i = startDegree; i <= endDegree; i++)
	{
		float degInRad = i * DEG2RAD;
		glVertex2f(x + xradius + cos(degInRad)*xradius, y + yradius + sin(degInRad)*yradius);
	}
	glEnd();
}

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

void renderBackground() {
	glColor3f(0.6f, 0.851f, 0.918f);
	glRectf(0.0f, 0.0f, 192.0f, 108.0f);
}

void renderPikachuBase(float width, float height) {
	glColor3f(1.0f, 1.0f, 0.0f);
	drawEllipse(0.0f, height - width, width / 2, width / 2);
	drawEllipse(0.0f, height * 0.03f, width / 2, height * 0.3f);
}

void renderPikachuChick(float width, float height) {
	glColor3f(1.0f, 0.0f, 0.0f);
	drawEllipse(width * 3 / 5, height * 3 / 5, width / 8, width / 8);
}

void renderPikachuEye(float width, float height) {
	glColor3f(0.0f, 0.0f, 0.0f);
	drawEllipse(width * 3 / 4, height * 3 / 4, width / 16, width / 10);
	glColor3f(1.0f, 1.0f, 1.0f);
	drawEllipse(width * 4 / 5, height * 4 / 5, width / 32, width / 20);
}

void renderPikachuNose(float width, float height) {
	glColor3f(0.0f, 0.0f, 0.0f);
	drawEllipse(width * 19 / 20, height * 5 / 7, width / 20, width / 20);
}

void renderPikachuMouth(float width, float height) {
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	{
		glVertex3f(width * 0.9f, height * 9 / 14, 0.0f);
		glVertex3f(width * 0.97f, height * 9 / 14, 0.0f);
	}
	glEnd();
}

void renderPikachuFace(float width, float height) {
	renderPikachuChick(width, height);
	renderPikachuEye(width, height);
	renderPikachuNose(width, height);
	renderPikachuMouth(width, height);
}

void renderPikachuFoot(float width, float height) {
	glColor3f(1.0f, 1.0f, 0.0f);
	drawEllipse(width * 0.3f, 0.0f, width * 0.3f, height / 28);
}

void renderPikachuHand(float width, float height) {
	glTranslatef(width * 0.7f, height * 0.3f, 0.0f);
	glRotatef(135.0f, 0.0f, 0.0f, 1.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	drawEllipseStroke(0.0f, 0.0f, width * 0.25f, height * 0.05f, 90, 270);

	glRotatef(-135.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-width * 0.7f, -height * 0.3f, 0.0f);
}

void renderPikachu(void) {
	glm::vec2 size = glm::vec2(20.0f, 35.0f);

	renderPikachuBase(size.x, size.y);
	renderPikachuFace(size.x, size.y);
	renderPikachuFoot(size.x, size.y);
	renderPikachuHand(size.x, size.y);
}

void renderPikachuEar(void) {
	float width = 20.0f;
	float height = 35.0f;

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(width / 4, 0.0f, 0.0f);
		glVertex3f(width / 8, height / 4, 0.0f);
	}
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(width / 16, height / 8, 0.0f);
		glVertex3f(width * 3 / 16, height / 8, 0.0f);
		glVertex3f(width / 8, height / 4, 0.0f);
	}
	glEnd();
}

void renderPikachuProximalTail(void) {
	float width = 20.0f;
	float height = 35.0f;

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, height * 0.3f, 0.0f);
		glVertex3f(width * 0.2f, height * 0.3f, 0.0f);
		glVertex3f(width * 0.1f, 0.0f, 0.0f);
	}
	glEnd();

	glColor3f(0.624f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, height * 0.15f, 0.0f);
		glVertex3f(width * 0.15f, height * 0.15f, 0.0f);
		glVertex3f(width * 0.1f, 0.0f, 0.0f);
	}
	glEnd();
}

void renderPikachuDistalTail(void) {
	float width = 20.0f;
	float height = 35.0f;

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0f, height * 0.2f, 0.0f);
		glVertex3f(width * 0.25f, height * 0.35f, 0.0f);
		glVertex3f(width * 0.15f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
	}
	glEnd();
}

void renderPikachuEarOld(float width, float height) {
	glTranslatef(width / 5, height * 6 / 7, 0.0f);
	glRotatef(50.0f, 0.0f, 0.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(width / 4, 0.0f, 0.0f);
		glVertex3f(width / 8, height / 4, 0.0f);
	}
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(width / 16, height / 8, 0.0f);
		glVertex3f(width * 3 / 16, height / 8, 0.0f);
		glVertex3f(width / 8, height / 4, 0.0f);
	}
	glEnd();

	glRotatef(-50.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-width / 5, -height * 6 / 7, 0.0f);
}

void renderPikachuTail(float width, float height) {
	glTranslatef(width * 0.05f, height * 0.2f, 0.0f);
	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, height * 0.3f, 0.0f);
		glVertex3f(width * 0.2f, height * 0.3f, 0.0f);
		glVertex3f(width * 0.1f, 0.0f, 0.0f);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glVertex3f(width * 0.1f, height * 0.4f, 0.0f);
		glVertex3f(width * 0.35f, height * 0.55f, 0.0f);
		glVertex3f(width * 0.25f, height * 0.2f, 0.0f);
		glVertex3f(width * 0.1f, height * 0.2f, 0.0f);
	}
	glEnd();
	glColor3f(0.624f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, height * 0.15f, 0.0f);
		glVertex3f(width * 0.15f, height * 0.15f, 0.0f);
		glVertex3f(width * 0.1f, 0.0f, 0.0f);
	}
	glEnd();

	glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-width * 0.05f, -height * 0.2f, 0.0f);
}

void renderPikachuOld(MovableRectangleObject player, bool isLeft) {
	glm::vec2 pos = player.getPosition();
	glm::vec2 size = player.getSize();
	
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(pos.x, pos.y, 0.0);
	if (!isLeft) {
		glTranslatef(size.x, 0.0, 0.0);
		glScalef(-1.0, 1.0, 0.0);
	}

	renderPikachuBase(size.x, size.y);
	renderPikachuFace(size.x, size.y);
	renderPikachuEarOld(size.x, size.y);
	renderPikachuFoot(size.x, size.y);
	renderPikachuTail(size.x, size.y);
	renderPikachuHand(size.x, size.y);

	if (!isLeft) {
		glScalef(-1.0, 1.0, 0.0);
		glTranslatef(-size.x, 0.0, 0.0);
	}
	glTranslatef(-pos.x, -pos.y, 0.0);
}

void renderNet(RectangleObject net) {
	glm::vec2 pos = net.getPosition();
	glm::vec2 size = net.getSize();

	glTranslatef(pos.x, pos.y, 0.0f);

	glColor3f(0.6f, 0.6f, 0.6f);
	glRectf(0.0f, 0.0f, size.x, size.y);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, size.y, 0.0f);
		glVertex3f(size.x, size.y, 0.0f);
		glVertex3f(size.x, 0.0f, 0.0f);
	}
	glEnd();
	glColor3f(0.8f, 0.8f, 0.8f);
	drawEllipse(0.0f, size.y - size.x, size.x * 0.5f, size.x * 0.5f);
	glColor3f(0.7f, 0.7f, 0.7f);
	drawEllipse(size.x * 0.25f, size.y - size.x * 0.75f, size.x * 0.25f, size.x * 0.25f);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawEllipseStroke(0.0f, size.y - size.x, size.x * 0.5f, size.x * 0.5f);
	drawEllipseStroke(size.x * 0.25f, size.y - size.x * 0.75f, size.x * 0.25f, size.x * 0.25f);

	glTranslatef(-pos.x, -pos.y, 0.0);
}

void renderBall(BallObject ball) {
	glm::vec2 pos = ball.getPosition();
	float radius = ball.getRadius();
	glColor3f(1.0f, 0.0f, 0.0f);
	drawEllipse(pos.x, pos.y, radius, radius, 0, 180);
	glColor3f(0.6f, 0.6f, 0.6f);
	drawEllipse(pos.x, pos.y, radius, radius, 180, 360);
	glColor3f(0.8f, 0.0f, 0.0f);
	glRectf(pos.x, pos.y + radius * 0.9f, pos.x + radius * 2, pos.y + radius * 1.1f);
	drawEllipse(pos.x + radius * 0.58f, pos.y + radius * 0.58f, radius * 0.42f, radius * 0.42f);
	glColor3f(0.6f, 0.6f, 0.6f);
	drawEllipse(pos.x + radius * 0.8f, pos.y + radius * 0.8f, radius * 0.2f, radius * 0.2f);
	glColor3f(0.0f, 0.0f, 0.0f);
	drawEllipseStroke(pos.x + radius * 0.8f, pos.y + radius * 0.8f, radius * 0.2f, radius * 0.2f);
	drawEllipseStroke(pos.x + radius * 0.9f, pos.y + radius * 0.9f, radius * 0.1f, radius * 0.1f);
	drawEllipseStroke(pos.x, pos.y, radius, radius);
}