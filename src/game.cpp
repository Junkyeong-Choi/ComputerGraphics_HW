#include "game.h"
#include <iostream>
#include <glm/geometric.hpp>

using namespace std;

const float DEG2RAD = 3.141592 / 180.0;

// code from https://forums.khronos.org/showthread.php/19787
void drawEllipse(float x, float y, float xradius, float yradius)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float degInRad = i * DEG2RAD;
		glVertex2f(x + xradius + cos(degInRad)*xradius, y + yradius + sin(degInRad)*yradius);
	}
	glEnd();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

bool Game::isExiting() {
	return false;
}

void Game::update(int delta) {
	return;
}

void Game::render() {
	glLoadIdentity();
	gluOrtho2D(0.0, 192.0, 0.0, 108.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glm::vec2 pos = player1.getPosition();
	glm::vec2 size = player1.getSize();
	glColor3f(1.0, 1.0, 0.0);
	glRectf(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	pos = player2.getPosition();
	size = player2.getSize();
	glColor3f(1.0, 1.0, 0.0);
	glRectf(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	pos = net.getPosition();
	size = net.getSize();
	glColor3f(0.8, 0.1, 0.1);
	glRectf(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

	pos = ball.getPosition();
	float radius = ball.getRadius();
	glColor3f(0.9, 0.0, 0.0);
	drawEllipse(pos.x, pos.y, radius, radius);

	glutSwapBuffers();
}

Game::Game(int argc, char* argv[], int width, int height, bool isFullScreen) {
	player1 = MovableRectangleObject(glm::vec2(24, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	player2 = MovableRectangleObject(glm::vec2(148, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	ball = BallObject(glm::vec2(90, 70), 7.5, glm::vec2(0, 0));
	net = RectangleObject(glm::vec2(90, 0), glm::vec2(5, 50));
	gamestate = GAME_MENU;

	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pickachu Volleyball");
	if (isFullScreen)
		glutFullScreen();

	glutReshapeFunc(reshape);

	glClearColor(0.6, 0.851, 0.918, 0.0);
	glShadeModel(GL_FLAT);
}


Direction Game::vectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	float max = 0.0f;
	int match = -1;
	for (int i = 0; i < 4; i++) {
		float dot_product = glm::dot(target, compass[i]);
		if (dot_product > max) {
			max = dot_product;
			match = i;
		}
	}
	return (Direction)match;
}

Collision Game::CheckCollision(RectangleObject rect, BallObject ball) {
	glm::vec2 center(ball.getPosition() + ball.getRadius());

	glm::vec2 aabb_half_extents(rect.getSize() / 2.0f);
	glm::vec2 aabb_center(rect.getPosition().x + aabb_half_extents.x, rect.getPosition().y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;
	difference = closest - center;

	if (glm::length(difference) < ball.getRadius())
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0, 0));
}

Collision Game::CheckCollision(RectangleObject fixed_rect, MovableRectangleObject movable_rect) {

	glm::vec2 fixed_center(fixed_rect.getPosition() + fixed_rect.getSize() / 2.0f);
	glm::vec2 movable_center(movable_rect.getPosition() + movable_rect.getSize() / 2.0f);
	glm::vec2 fixed_half_extents(fixed_rect.getSize() / 2.0f);

	glm::vec2 difference = movable_center - fixed_center;
	glm::vec2 clamped = glm::clamp(difference, -fixed_half_extents, fixed_half_extents);
	glm::vec2 closest = fixed_center + clamped;
	difference = closest - movable_center;

	bool collisionX = fixed_rect.getPosition().x + fixed_rect.getSize().x >= movable_rect.getPosition().x &&
		movable_rect.getPosition().x + movable_rect.getSize().x >= fixed_rect.getPosition().x;
	bool collisionY = fixed_rect.getPosition().y + fixed_rect.getSize().y >= movable_rect.getPosition().y &&
		movable_rect.getPosition().y + movable_rect.getSize().y >= fixed_rect.getPosition().y;

	if (collisionX && collisionY)
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0, 0));
}