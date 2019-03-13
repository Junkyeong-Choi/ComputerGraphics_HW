#include "game.h"
#include "render.h"
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

void Game::exit() {
	exiting = true;
}

bool Game::isExiting() {
	return exiting;
}

void Game::render() {
	setCamera(ballCameraMode, ball);

	glColor3f(0.6, 0.851, 0.918);
	glRectf(0.0, 0.0, 192.0, 108.0);
	
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


Game::Game():
	player1(glm::vec2(24, 0), glm::vec2(20, 35), glm::vec2(0, 0)),
	player2(glm::vec2(148, 0), glm::vec2(20, 35), glm::vec2(0, 0)),
	ball(glm::vec2(90, 70), 7.5, glm::vec2(0.1, 0.1)),
	net(glm::vec2(90, 0), glm::vec2(5, 50)),
	gamestate(GAME_MENU),
	ballCameraMode(false),
	score1(0), score2(0)
{}

void Game::init(int argc, char* argv[], int width, int height, bool isFullScreen) {
	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pickachu Volleyball");
	if (isFullScreen)
		glutFullScreen();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void Game::handleInput(unsigned char key) {
	glm::vec2 velocity(0.0, 0.0);

	switch (key) {
	case 'a':
		velocity.x = -0.1;
		break;
	case 'd':
		velocity.x = 0.1;
		break;
	case ' ':
		ballCameraMode = !ballCameraMode;
		return;
	}

	player1.setVelocity(velocity);
}

void Game::handleInputUp(unsigned char key) {
	glm::vec2 velocity = player1.getVelocity();

	switch (key) {
	case 'a':
		if (velocity.x < 0)
			velocity.x = 0;
		break;
	case 'd':
		if (velocity.x > 0)
			velocity.x = 0;
		break;
	}

	player1.setVelocity(velocity);
}

void Game::handleSpecialInput(int key) {
	glm::vec2 velocity(0.0, 0.0);

	switch (key) {
	case GLUT_KEY_LEFT:
		velocity.x = -0.1;
		break;
	case GLUT_KEY_RIGHT:
		velocity.x = 0.1;
		break;
	}

	player2.setVelocity(velocity);
}

void Game::handleSpecialInputUp(int key) {
	glm::vec2 velocity = player2.getVelocity();

	switch (key) {
	case GLUT_KEY_LEFT:
		if (velocity.x < 0)
			velocity.x = 0;
		break;
	case GLUT_KEY_RIGHT:
		if (velocity.x > 0)
			velocity.x = 0;
		break;
	}

	player2.setVelocity(velocity);
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

void Game::updateBall(int delta) {
	RectangleObject* objectsToCollideAgainstBall[3] = { &player1, &player2, &net };

	ball.move(delta);

	for (size_t i = 0; i < 3; i++) {
		Collision collision = CheckCollision(*objectsToCollideAgainstBall[i], ball);
		if (std::get<0>(collision)) {
			Direction dir = std::get<1>(collision);
			glm::vec2 diff_vec = std::get<2>(collision);
			glm::vec2 ballVelocity = ball.getVelocity();
			glm::vec2 ballPosition = ball.getPosition();
			float ballRadius = ball.getRadius();

			if (dir == LEFT || dir == RIGHT) {
				ballVelocity.x = -ballVelocity.x;
				float penetration = ballRadius - std::abs(diff_vec.x);
				if (dir == LEFT)
					ballPosition.x += penetration;
				else
					ballPosition.x -= penetration;
			}
			else {
				ballVelocity.y = -ballVelocity.y;
				float penetration = ball.getRadius() - std::abs(diff_vec.y);
				if (dir == DOWN)
					ballPosition.y += penetration;
				else
					ballPosition.y -= penetration;
			}
			ball.setVelocity(ballVelocity);
			ball.setPosition(ballPosition);
		}
	}

}

void Game::updatePlayer(int delta) {
	MovableRectangleObject* players[2] = { &player1, &player2 };

	for (size_t i = 0; i < 2; i++) {
		players[i]->move(delta);
	}

	for (size_t i = 0; i < 2; i++) {
		Collision collision = CheckCollision(net, *players[i]);
		if (std::get<0>(collision)) {
			Direction dir = std::get<1>(collision);
			glm::vec2 diff_vec = std::get<2>(collision);
			glm::vec2 objectVelocity = players[i]->getVelocity();
			glm::vec2 objectPosition = players[i]->getPosition();

			float penetration = players[i]->getSize().x / 2 - std::abs(diff_vec.x);
			if (dir == LEFT)
				objectPosition.x += penetration;
			else
				objectPosition.x -= penetration;

			players[i]->setVelocity(objectVelocity);
			players[i]->setPosition(objectPosition);
		}
	}
	glm::vec2 ballPosition = ball.getPosition();
	if (ballPosition.y < 28) {
		glm::vec2 player1Position = player1.getPosition();
		glm::vec2 player2Position = player2.getPosition();
		if (ballPosition.x < 1 && player1Position.x < 16)
			player1Position.x = 16;
		if (ballPosition.x > 74 && ballPosition.x < 90 && player1Position.x > 54)
			player1Position.x = 54;
		if (ballPosition.x > 90 && ballPosition.x < 96 && player2Position.x < 121)
			player2Position.x = 121;
		if (ballPosition.x > 176 && player2Position.x > 156)
			player2Position.x = 156;
		player1.setPosition(player1Position);
		player2.setPosition(player2Position);
	}
	
}

void Game::update(int delta) {
	
	updateBall(delta);
	
	updatePlayer(delta);

	if (ball.getPosition().y < 2) {
		if (ball.getPosition().x < 92)
			score1++;
		if (ball.getPosition().x > 92)
			score2++;
		ball.setPosition(glm::vec2(90, 70));
	}

	return;
}