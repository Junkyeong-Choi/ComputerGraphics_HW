#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/geometric.hpp>
#include "ballObject.h"
#include "movableRectangleObject.h"
#include "game.h"

const float DEG2RAD = 3.141592 / 180.0;

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

void renderBackground(GameState gamestate) {
	if (gamestate == GAME_MENU)
		glColor3f(1.0, 1.0, 0.0);
	else
		glColor3f(0.6, 0.851, 0.918);
	glRectf(0.0, 0.0, 192.0, 108.0);
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

	glRasterPos2i(width * x - length / 2, height * y - 9);
	glutBitmapString(font, reinterpret_cast<const unsigned char *>(text));
}

void renderScore(int score1, int score2) {
	char buffer[10];
	glColor3f(0.0, 0.0, 0.0);
	_itoa_s(score1, buffer, 10);
	renderText(0.1, 0.9, buffer);
	_itoa_s(score2, buffer, 10);
	renderText(0.9, 0.9, buffer);
}

void renderReady(int delayTime) {
	char buffer[10];
	_itoa_s(delayTime / 1000 + 1, buffer, 10);
	glColor3f(1.0, 0.0, 0.0);
	renderText(0.5, 0.9, buffer);
}

void renderGo() {
	glColor3f(1.0, 0.0, 0.0);
	renderText(0.5, 0.9, "GO!");
}

void renderScoreText() {
	glColor3f(1.0, 0.0, 0.0);
	renderText(0.5, 0.9, "SCORE!");
}

void renderCameraText(bool ballCameraMode) {
	glColor3f(0.4, 0.4, 0.4);

	if (ballCameraMode)
		renderText(0.5, 0.9, "Ball Camera (Spacebar to change)");
	else
		renderText(0.5, 0.9, "Normal Camera (Spacebar to change)");
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
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(192.0 * 0.25, 108.0 * 0.5, 0.0);

	renderMenuPikachuHalf();
	glScalef(-1.0, 1.0, 0.0);
	renderMenuPikachuHalf();
	glScalef(-1.0, 1.0, 0.0);

	glTranslatef(-192.0 * 0.25, -108.0 * 0.5, 0.0);

	glColor3f(0.8, 0.0, 0.0);
	renderText(0.75, 0.65, "PIKACHU VOLLEYBALL");

	if (is2player)
		glColor3f(0.4, 0.4, 0.4);
	else
		glColor3f(0.0, 0.0, 0.0);
	renderText(0.75, 0.45, "Single Play");

	if (is2player)
		glColor3f(0.0, 0.0, 0.0);
	else
		glColor3f(0.4, 0.4, 0.4);
	renderText(0.75, 0.35, "Multi Play");
}

void renderWinText(bool is2player, int score1, int score2) {
	glColor3f(1.0, 0.0, 0.0);

	if (is2player) {
		if (score1 > score2)
			renderText(0.5, 0.9, "PLAYER 1 WIN!");
		else
			renderText(0.5, 0.9, "PLAYER 2 WIN!");
	}
	else {
		if (score1 > score2)
			renderText(0.5, 0.9, "YOU WIN!");
		else
			renderText(0.5, 0.9, "YOU LOSE ;(");
	}

	glColor3f(0.4, 0.4, 0.4);
	renderText(0.5, 0.8, "(Press Enter)");
}

void renderPikachuBase(float width, float height) {
	glColor3f(1.0, 1.0, 0.0);
	drawEllipse(0.0, height - width, width / 2, width / 2);
	drawEllipse(0.0, height * 0.03, width / 2, height * 0.3);
}

void renderPikachuChick(float width, float height) {
	glColor3f(1.0, 0.0, 0.0);
	drawEllipse(width * 3 / 5, height * 3 / 5, width / 8, width / 8);
}

void renderPikachuEye(float width, float height) {
	glColor3f(0.0, 0.0, 0.0);
	drawEllipse(width * 3 / 4, height * 3 / 4, width / 16, width / 10);
	glColor3f(1.0, 1.0, 1.0);
	drawEllipse(width * 4 / 5, height * 4 / 5, width / 32, width / 20);
}

void renderPikachuNose(float width, float height) {
	glColor3f(0.0, 0.0, 0.0);
	drawEllipse(width * 19 / 20, height * 5 / 7, width / 20, width / 20);
}

void renderPikachuMouth(float width, float height) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	{
		glVertex3f(width * 0.9, height * 9 / 14, 0.0);
		glVertex3f(width * 0.97, height * 9 / 14, 0.0);
	}
	glEnd();
}

void renderPikachuFace(float width, float height) {
	renderPikachuChick(width, height);
	renderPikachuEye(width, height);
	renderPikachuNose(width, height);
	renderPikachuMouth(width, height);
}

void renderPikachuEar(float width, float height) {
	glTranslatef(width / 5, height * 6 / 7, 0.0);
	glRotatef(50.0, 0.0, 0.0, 1.0);

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(width / 4, 0.0, 0.0);
		glVertex3f(width / 8, height / 4, 0.0);
	}
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	{
		glVertex3f(width / 16, height / 8, 0.0);
		glVertex3f(width * 3 / 16, height / 8, 0.0);
		glVertex3f(width / 8, height / 4, 0.0);
	}
	glEnd();

	glRotatef(-50.0, 0.0, 0.0, 1.0);
	glTranslatef(-width / 5, -height * 6 / 7, 0.0);
}

void renderPikachuFoot(float width, float height) {
	glColor3f(1.0, 1.0, 0.0);
	drawEllipse(width * 0.3, 0, width * 0.3, height / 28);
}

void renderPikachuTail(float width, float height) {
	glTranslatef(width * 0.05, height * 0.2, 0.0);
	glRotatef(60.0, 0.0, 0.0, 1.0);

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, height * 0.3, 0.0);
		glVertex3f(width * 0.2, height * 0.3, 0.0);
		glVertex3f(width * 0.1, 0.0, 0.0);
	}
	glEnd();
	glBegin(GL_POLYGON);
	{
		glVertex3f(width * 0.1, height * 0.4, 0.0);
		glVertex3f(width * 0.35, height * 0.55, 0.0);
		glVertex3f(width * 0.25, height * 0.2, 0.0);
		glVertex3f(width * 0.1, height * 0.2, 0.0);
	}
	glEnd();
	glColor3f(0.624, 0.0, 0.0);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, height * 0.15, 0.0);
		glVertex3f(width * 0.15, height * 0.15, 0.0);
		glVertex3f(width * 0.1, 0.0, 0.0);
	}
	glEnd();

	glRotatef(-60.0, 0.0, 0.0, 1.0);
	glTranslatef(-width * 0.05, -height * 0.2, 0.0);
}

void renderPikachuHand(float width, float height) {
	glTranslatef(width * 0.7, height * 0.3, 0.0);
	glRotatef(135.0, 0.0, 0.0, 1.0);

	glColor3f(0.0, 0.0, 0.0);
	drawEllipseStroke(0.0, 0.0, width * 0.25, height * 0.05, 90, 270);

	glRotatef(-135.0, 0.0, 0.0, 1.0);
	glTranslatef(-width * 0.7, -height * 0.3, 0.0);
}

void renderPikachu(MovableRectangleObject player, bool isLeft) {
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
	renderPikachuEar(size.x, size.y);
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

	glTranslatef(pos.x, pos.y, 0.0);

	glColor3f(0.6, 0.6, 0.6);
	glRectf(0, 0, size.x, size.y);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, size.y, 0.0);
		glVertex3f(size.x, size.y, 0.0);
		glVertex3f(size.x, 0.0, 0.0);
	}
	glEnd();
	glColor3f(0.8, 0.8, 0.8);
	drawEllipse(0.0, size.y - size.x, size.x * 0.5, size.x * 0.5);
	glColor3f(0.7, 0.7, 0.7);
	drawEllipse(size.x * 0.25, size.y - size.x * 0.75, size.x * 0.25, size.x * 0.25);
	glColor3f(0.0, 0.0, 0.0);
	drawEllipseStroke(0.0, size.y - size.x, size.x * 0.5, size.x * 0.5);
	drawEllipseStroke(size.x * 0.25, size.y - size.x * 0.75, size.x * 0.25, size.x * 0.25);

	glTranslatef(-pos.x, -pos.y, 0.0);
}

void renderBall(BallObject ball) {
	glm::vec2 pos = ball.getPosition();
	float radius = ball.getRadius();
	glColor3f(1.0, 0.0, 0.0);
	drawEllipse(pos.x, pos.y, radius, radius, 0, 180);
	glColor3f(0.6, 0.6, 0.6);
	drawEllipse(pos.x, pos.y, radius, radius, 180, 360);
	glColor3f(0.8, 0.0, 0.0);
	glRectf(pos.x, pos.y + radius * 0.9, pos.x + radius * 2, pos.y + radius * 1.1);
	drawEllipse(pos.x + radius * 0.58, pos.y + radius * 0.58, radius * 0.42, radius * 0.42);
	glColor3f(0.6, 0.6, 0.6);
	drawEllipse(pos.x + radius * 0.8, pos.y + radius * 0.8, radius * 0.2, radius * 0.2);
	glColor3f(0.0, 0.0, 0.0);
	drawEllipseStroke(pos.x + radius * 0.8, pos.y + radius * 0.8, radius * 0.2, radius * 0.2);
	drawEllipseStroke(pos.x + radius * 0.9, pos.y + radius * 0.9, radius * 0.1, radius * 0.1);
	drawEllipseStroke(pos.x, pos.y, radius, radius);
}