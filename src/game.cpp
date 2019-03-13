#include "game.h"
#include "render.h"
#include <iostream>
#include <algorithm>
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
	if (ballCameraMode)
		setBallCamera(ball.getPosition(), ball.getRadius());
	else
		setNormalCamera();

	// draw background color
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

	renderScore(score1, score2);

	glutSwapBuffers();
}


Game::Game() :
	player1(glm::vec2(24, 0), glm::vec2(20, 35), glm::vec2(0, 0)),
	player2(glm::vec2(148, 0), glm::vec2(20, 35), glm::vec2(0, 0)),
	ball(glm::vec2(88.5, 70), 7.5, glm::vec2(0.1, 0.1)),
	net(glm::vec2(93.5, 0), glm::vec2(5, 50)),
	gamestate(GAME_READY),
	ballCameraMode(false),
	delayTime(3000),
	is2player(false),
	score1(0), score2(0),
	winningScore(15)
{}

void Game::resetPosition() {
	player1.setPosition(glm::vec2(24, 0));
	player1.setsize(glm::vec2(20, 35));
	player1.setVelocity(glm::vec2(0, 0));

	player2.setPosition(glm::vec2(148, 0));
	player2.setsize(glm::vec2(20, 35));
	player2.setVelocity(glm::vec2(0, 0));

	ball.setPosition(glm::vec2(88.5, 70));
	ball.setRadius(7.5);
	ball.setVelocity(glm::vec2(0.1, 0.1));
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

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void Game::handleInput(unsigned char key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
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

	if (key == ' ')
		ballCameraMode = !ballCameraMode;
}

void Game::handleInputUp(unsigned char key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
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
}

void Game::handleSpecialInput(int key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		if (!is2player)
			return;

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
}

void Game::handleSpecialInputUp(int key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		if (!is2player)
			return;

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
		float dotProduct = glm::dot(target, compass[i]);
		if (dotProduct > max) {
			max = dotProduct;
			match = i;
		}
	}
	return (Direction)match;
}

Collision Game::CheckCollision(RectangleObject rect, BallObject ball) {
	glm::vec2 center(ball.getPosition() + ball.getRadius());

	glm::vec2 aabbHalfExtents(rect.getSize() / 2.0f);
	glm::vec2 aabbCenter(rect.getPosition().x + aabbHalfExtents.x, rect.getPosition().y + aabbHalfExtents.y);

	glm::vec2 difference = center - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);

	glm::vec2 closest = aabbCenter + clamped;
	difference = closest - center;

	if (glm::length(difference) < ball.getRadius())
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0, 0));
}

Collision Game::CheckCollision(RectangleObject fixedRect, MovableRectangleObject movableRect) {

	glm::vec2 fixedRectPosition = fixedRect.getPosition();
	glm::vec2 fixedRectSize = fixedRect.getSize();
	glm::vec2 movableRectPosition = movableRect.getPosition();
	glm::vec2 movableRectSize = movableRect.getSize();
	
	bool collisionX = fixedRectPosition.x + fixedRectSize.x >= movableRectPosition.x &&
		movableRectPosition.x + movableRectSize.x >= fixedRectPosition.x;
	bool collisionY = fixedRectPosition.y + fixedRectSize.y >= movableRectPosition.y &&
		movableRectPosition.y + movableRectSize.y >= fixedRectPosition.y;
	
	if (collisionX && collisionY) {
		float collidingBox_x = std::min({ fixedRectPosition.x + fixedRectSize.x - movableRectPosition.x,
		movableRectPosition.x + movableRectSize.x - fixedRectPosition.x,
		fixedRectSize.x, movableRectSize.x });
		
		float collidingBox_y = std::min({ fixedRectPosition.y + fixedRectSize.y - movableRectPosition.y,
			movableRectPosition.y + movableRectSize.y - fixedRectPosition.y,
			fixedRectSize.y, movableRectSize.y });

		Direction direction;
		if (collidingBox_x < collidingBox_y) {
			if (fixedRectPosition.x > movableRectPosition.x)
				direction = RIGHT;
			else
				direction = LEFT;
		}
		else {
			if (fixedRectPosition.y > movableRectPosition.y)
				direction = UP;
			else
				direction = DOWN;
		}
		return std::make_tuple(true, direction, glm::vec2(collidingBox_x, collidingBox_y));
	}
	
	return std::make_tuple(false, UP, glm::vec2(0, 0));
}

void Game::updateBall(int delta) {
	RectangleObject* objectsToCollideAgainstBall[3] = { &player1, &player2, &net };

	ball.move(delta);

	for (size_t i = 0; i < 3; i++) {
		Collision collision = CheckCollision(*objectsToCollideAgainstBall[i], ball);
		if (std::get<0>(collision)) {
			Direction dir = std::get<1>(collision);
			glm::vec2 diffVec = std::get<2>(collision);
			glm::vec2 ballVelocity = ball.getVelocity();
			glm::vec2 ballPosition = ball.getPosition();
			float ballRadius = ball.getRadius();

			if (dir == LEFT || dir == RIGHT) {
				ballVelocity.x = -ballVelocity.x;
				float penetration = ballRadius - std::abs(diffVec.x);
				if (dir == LEFT)
					ballPosition.x += penetration;
				else
					ballPosition.x -= penetration;
			}
			else {
				ballVelocity.y = -ballVelocity.y;
				float penetration = ball.getRadius() - std::abs(diffVec.y);
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
			glm::vec2 collidingBox = std::get<2>(collision);
			glm::vec2 objectPosition = players[i]->getPosition();
			if (dir == LEFT || dir == RIGHT) {
				float penetration = collidingBox.x;
				if (dir == LEFT)
					objectPosition.x += penetration;
				else
					objectPosition.x -= penetration;

			}
			else {
				float penetration = collidingBox.y;
				if (dir == DOWN)
					objectPosition.x += penetration;
				else
					objectPosition.x -= penetration;
			}
			players[i]->setPosition(objectPosition);
		}
	}
	glm::vec2 ballPosition = ball.getPosition();
	glm::vec2 netPosition = net.getPosition();
	float ballRadius = ball.getRadius();
	glm::vec2 netSize = net.getSize();
	glm::vec2 player1Size = player1.getSize();
	glm::vec2 player2Size = player2.getSize();

	int epsilon = 3;

	if (ballPosition.y < netSize.y - ballRadius) {
		glm::vec2 player1Position = player1.getPosition();
		glm::vec2 player2Position = player2.getPosition();
		if (ballPosition.x <= epsilon && player1Position.x <= ballRadius * 2)
			player1Position.x = ballRadius * 2 + epsilon;
		if (ballPosition.x >= (netPosition.x - ballRadius * 2 - epsilon) && ballPosition.x <= (netPosition.x - ballRadius * 2) && player1Position.x >= (netPosition.x - ballRadius * 2 - player1Size.x))
			player1Position.x = netPosition.x - ballRadius * 2 - player1Size.x - epsilon;
		if (ballPosition.x >= (netPosition.x + netSize.x) && ballPosition.x <= (netPosition.x + netSize.x + epsilon) && player2Position.x <= (netPosition.x + netSize.x + ballRadius * 2))
			player2Position.x = netPosition.x + netSize.x + ballRadius * 2 + epsilon;
		if (ballPosition.x >= 192 - ballRadius * 2 - epsilon && player2Position.x >= 192 - ballRadius * 2 - player2Size.x)
			player2Position.x = 192 - ballRadius * 2 - player2Size.x - epsilon;
		player1.setPosition(player1Position);
		player2.setPosition(player2Position);
	}
	
}

void Game::update(int delta) {

	if (gamestate == GAME_READY) {
		delayTime -= delta;
		if (delayTime < 0) {
			delayTime = 1000;
			gamestate = GAME_PLAYING;
		}
	}

	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		if (!is2player) {
			glm::vec2 velocity(0.0, 0.0);

			if (player2.getPosition().x < ball.getPosition().x)
				velocity.x = PLAYER_MAX_VELOCITY * 0.83;
			else
				velocity.x = -PLAYER_MAX_VELOCITY * 0.83;

			player2.setVelocity(velocity);
		}
	}

	if (gamestate == GAME_PLAYING) {
		updateBall(delta);
		updatePlayer(delta);

		if (ball.getPosition().y < 2) {
			if (ball.getPosition().x < 92)
				score2++;
			else
				score1++;

			if (score1 == winningScore || score2 == winningScore) {
				gamestate = GAME_SET;
				delayTime = 3000;
			}
			else {
				gamestate = GAME_SCORE;
				delayTime = 3000;
			}
		}
	}
	else if (gamestate == GAME_SCORE) {
		updateBall(delta / 2);
		updatePlayer(delta / 2);
		delayTime -= delta;

		if (delayTime < 0) {
			delayTime = 3000;
			gamestate = GAME_READY;
			resetPosition();
		}
	}

	return;
}