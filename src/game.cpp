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

bool Game::isExiting() {
	return false;
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

Game::Game() {
	player1 = MovableRectangleObject(glm::vec2(24, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	player2 = MovableRectangleObject(glm::vec2(148, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	ball = BallObject(glm::vec2(90, 70), 7.5, glm::vec2(0.1, 0.1));
	net = RectangleObject(glm::vec2(90, 0), glm::vec2(5, 50));
	gamestate = GAME_MENU;
}

void Game::init(int argc, char* argv[], int width, int height, bool isFullScreen) {
	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pickachu Volleyball");
	if (isFullScreen)
		glutFullScreen();

	glClearColor(0.6, 0.851, 0.918, 0.0);
	glShadeModel(GL_FLAT);
}

void Game::handleInput(unsigned char key) {
	cout << key << endl;
}

void Game::handleSpecialInput(int key) {
	cout << key << endl;
}

void Game::handleSpecialInputUp(int key) {
	cout << key << endl;
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

void Game::update(int delta) {
	//std::cout << delta << std::endl;
	RectangleObject* objectsToCollideAgainstBall[3] = {&player1, &player2, &net};
	MovableRectangleObject objectsToCollideAgainstNet[2] = { player1, player2 };

	for (size_t i = 0; i < 3; i++){
		Collision collision = CheckCollision(*objectsToCollideAgainstBall[i], ball);
		if (std::get<0>(collision)) {
			Direction dir = std::get<1>(collision);
			glm::vec2 diff_vec = std::get<2>(collision);
			if (dir == LEFT || dir == RIGHT) {
				glm::vec2 velocity = ball.getVelocity();
				velocity.x = -velocity.x;
				ball.setVelocity(velocity);
				float penetration = ball.getRadius() - std::abs(diff_vec.x);
				if (dir == LEFT) {
					glm::vec2 position = ball.getPosition();
					position.x += penetration;
					ball.setPosition(position);
				}
				else {
					glm::vec2 position = ball.getPosition();
					position.x -= penetration;
					ball.setPosition(position);
				}	
			}
			else {
				glm::vec2 velocity = ball.getVelocity();
				velocity.y = -velocity.y;
				ball.setVelocity(velocity);
				float penetration = ball.getRadius() - std::abs(diff_vec.y);
				if (dir == DOWN) {
					glm::vec2 position = ball.getPosition();
					position.y += penetration;
					ball.setPosition(position);
				}
				else {
					glm::vec2 position = ball.getPosition();
					position.y -= penetration;
					ball.setPosition(position);
				}
			}
		}
	}

	for (size_t i = 0; i < 2; i++) {
		Collision collision = CheckCollision(net, objectsToCollideAgainstNet[i]);
		if (std::get<0>(collision)) {
			Direction dir = std::get<1>(collision);
			glm::vec2 diff_vec = std::get<2>(collision);
			if (dir == LEFT || dir == RIGHT) {
				glm::vec2 velocity = objectsToCollideAgainstNet[i].getVelocity();
				velocity.x = -velocity.x;
				ball.setVelocity(velocity);
				float penetration = objectsToCollideAgainstNet[i].getSize().x / 2 - std::abs(diff_vec.x);
				if (dir == LEFT) {
					glm::vec2 position = objectsToCollideAgainstNet[i].getPosition();
					position.x += penetration;
					objectsToCollideAgainstNet[i].setPosition(position);
				}
				else {
					glm::vec2 position = objectsToCollideAgainstNet[i].getPosition();
					position.x -= penetration;
					objectsToCollideAgainstNet[i].setPosition(position);
				}
			}
			else {
				glm::vec2 velocity = objectsToCollideAgainstNet[i].getVelocity();
				velocity.y = -velocity.y;
				ball.setVelocity(velocity);
				float penetration = objectsToCollideAgainstNet[i].getSize().y / 2 - std::abs(diff_vec.y);
				if (dir == DOWN) {
					glm::vec2 position = objectsToCollideAgainstNet[i].getPosition();
					position.y += penetration;
					objectsToCollideAgainstNet[i].setPosition(position);
				}
				else {
					glm::vec2 position = objectsToCollideAgainstNet[i].getPosition();
					position.y -= penetration;
					objectsToCollideAgainstNet[i].setPosition(position);
				}
			}
		}
	}


	MovableRectangleObject players[2] = { player1, player2 };
	for (size_t i = 0; i < 2; i++) {
		glm::vec2 playerPosition = players[i].getPosition();
		glm::vec2 playerSize = players[i].getSize();
		if (playerPosition.x < 0) 
			playerPosition.x = 0;
		if (playerPosition.x + playerSize.x > 192) 
			playerPosition.x = 192 - playerSize.x;
		players[i].setPosition(playerPosition);
	}

	glm::vec2 ballPosition = ball.getPosition();
	glm::vec2 ballVelocity = ball.getVelocity();
	float ballRadius = ball.getRadius();
	if (ballPosition.x < 0 || ballPosition.x + 2 * ballRadius > 192) {
		ballVelocity.x = -ballVelocity.x;
		if (ballPosition.x < 0)
			ballPosition.x += 1;
		else
			ballPosition.x -= 1;
	}
	if (ballPosition.y < 0 || ballPosition.y + 2 * ballRadius > 108) {
		ballVelocity.y = -ballVelocity.y;
		if (ballPosition.y < 0) 
			ballPosition.y += 1;
		else
			ballPosition.y -= 1;
	}
	ball.setVelocity(ballVelocity);
	ball.setPosition(ballPosition);

	for (size_t i = 0; i < 2; i++) {
		glm::vec2 playerPosition = players[i].getPosition();
		glm::vec2 playerVelocity = players[i].getVelocity();
		glm::vec2 playerDisplacement = playerVelocity * (float)delta;
		playerPosition += playerDisplacement;
		players[i].setPosition(playerPosition);
	}
	ballPosition = ball.getPosition();
	ballVelocity = ball.getVelocity();
	glm::vec2 ballDisplacement = ballVelocity * (float)delta;
	ballPosition += ballDisplacement;
	ball.setPosition(ballPosition);

	return;
}