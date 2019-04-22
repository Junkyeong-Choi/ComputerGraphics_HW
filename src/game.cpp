#include "game.h"
#include "collision.h"
#include "sceneGraphNode.h"
#include <random>
#include <ctime>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer.h"
#include "ui.h"

void Game::exit() {
	exiting = true;
}

bool Game::isExiting() {
	return exiting;
}

void Game::render() {
	if (gamestate == GAME_MENU) {
		set2DCamera();
		renderMenu(is2player);
	}
	else {
		renderer.render();

		renderScore(score1, score2);

		if (gamestate == GAME_READY)
			renderReady(delayTime);
		else if (gamestate == GAME_PLAYING && delayTime > 0)
			renderGo();
		else if (gamestate == GAME_SCORE)
			renderScoreText();
		else if (gamestate == GAME_SET)
			renderWinText(is2player, score1, score2);
		else
			renderCameraText(ballCameraMode);
	}

	glutSwapBuffers();
}

float generateRandomFromZeroToOne() {
	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	return distribution(engine);
}

glm::vec3 generateUnitVector_old() {
	float x = 1;
	float y = 0;
	while (x < 0.4 || y < 0.2) {
		x = generateRandomFromZeroToOne();
		y = sqrt(1 - x * x);
	}

	return glm::vec3(x, y, 0);
}

glm::vec3 generateUnitVector() {
	float theta = 2 * PI * generateRandomFromZeroToOne();		// the rotation angle about z-axis (on xy plane)
	float phi = 2 * PI * generateRandomFromZeroToOne();			// the angle between z-axis and direction vector
	return glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi));
}

Game::Game() :
	player1(PLAYER_ONE_POSITION, PLAYER_ONE_SIZE, PLAYER_ONE_SPEED, PLAYER_ONE_DIRECTION_ANGLE, PLAYER_ONE_DIRECTION_ANGLE_VELOCITY),
	player2(PLAYER_TWO_POSITION, PLAYER_TWO_SIZE, PLAYER_TWO_SPEED, PLAYER_TWO_DIRECTION_ANGLE, PLAYER_TWO_DIRECTION_ANGLE_VELOCITY),
	ball(BALL_POSITION, BALL_RADIUS, BALL_SPEED * generateUnitVector_old()),
	gamestate(GAME_MENU),
	ballCameraMode(false),
	delayTime(3000),
	is2player(false),
	score1(0), score2(0), winningScore(5),
	player1Scored(false),
	renderer() {}

Game::~Game() {}

void Game::resetPosition() {
	delayTime = 3000;

	player1.setPosition(PLAYER_ONE_POSITION);
	player1.setsize(PLAYER_ONE_SIZE);
	player1.setSpeed(PLAYER_ONE_SPEED);
	player1.setDirectionAngle(PLAYER_ONE_DIRECTION_ANGLE);
	player1.setDirectionAngleVelocity(PLAYER_ONE_DIRECTION_ANGLE_VELOCITY);

	player2.setPosition(PLAYER_TWO_POSITION);
	player2.setsize(PLAYER_TWO_SIZE);
	player1.setSpeed(PLAYER_ONE_SPEED);
	player1.setDirectionAngle(PLAYER_ONE_DIRECTION_ANGLE);
	player1.setDirectionAngleVelocity(PLAYER_ONE_DIRECTION_ANGLE_VELOCITY);

	ball.setPosition(BALL_POSITION);
	ball.setRadius(BALL_RADIUS);
	glm::vec3 ballVelocity = BALL_SPEED * generateUnitVector_old();
	if ((player1Scored && ballVelocity.x < 0) || (!player1Scored && ballVelocity.x > 0))
		ballVelocity.x = -ballVelocity.x;
	ball.setVelocity(ballVelocity);
}

void Game::restartGame() {
	resetPosition();
	ballCameraMode = false;
	delayTime = 3000;
	score1 = 0;
	score2 = 0;
	player1Scored = false;
	gamestate = GAME_READY;
}

void Game::init(int argc, char* argv[], int width, int height, bool isFullScreen) {
	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitContextVersion(4, 6);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pickachu Volleyball");
	if (isFullScreen)
		glutFullScreen();

	renderer.setScreenSize(width, height);
}

void Game::initRenderer() {
	renderer.init();
}

void Game::handleInput(unsigned char key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		float speed = player1.getSpeed();
		glm::vec2 directionAngleVelocity = player1.getDirectionAngleVelocity();

		switch (key) {
		case 'a':
			directionAngleVelocity.x = 0.01f;
			break;
		case 'd':
			directionAngleVelocity.x = -0.01f;
			break;
		case 'w':
			speed = 0.1f;
			break;
		case 's':
			speed = -0.1f;
			break;
		case 'r':
			restartGame();
			break;
		case ' ':
			ballCameraMode = !ballCameraMode;
			return;
		}

		player1.setSpeed(speed);
		player1.setDirectionAngleVelocity(directionAngleVelocity);
	}
	else if (gamestate == GAME_MENU) {
		if (key == '\n' || key == 13)
			gamestate = GAME_READY;
	}
	else if (gamestate == GAME_SET) {
		if (key == '\n' || key == 13) {
			gamestate = GAME_MENU;
			score1 = 0;
			score2 = 0;
			resetPosition();
		}
	}

	if (key == ' ')
		ballCameraMode = !ballCameraMode;
}

void Game::handleInputUp(unsigned char key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		float speed = player1.getSpeed();
		glm::vec2 directionAngleVelocity = player1.getDirectionAngleVelocity();

		switch (key) {
		case 'a':
			if (directionAngleVelocity.x > 0)
				directionAngleVelocity.x = 0.0f;
			break;
		case 'd':
			if (directionAngleVelocity.x < 0)
				directionAngleVelocity.x = 0.0f;
			break;
		case 'w':
			if (speed > 0)
				speed = 0.0f;
			break;
		case 's':
			if (speed < 0)
				speed = 0.0f;
			break;
		}

		player1.setSpeed(speed);
		player1.setDirectionAngleVelocity(directionAngleVelocity);
	}

	if (key == 27) // ESC Key
		this->exit();
}

void Game::handleSpecialInput(int key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		if (!is2player)
			return;
		float speed = player2.getSpeed();
		glm::vec2 directionAngleVelocity = player2.getDirectionAngleVelocity();

		switch (key) {
		case GLUT_KEY_LEFT:
			directionAngleVelocity.x = 0.01f;
			break;
		case GLUT_KEY_RIGHT:
			directionAngleVelocity.x = -0.01f;
			break;
		case GLUT_KEY_UP:
			speed = 0.1f;
			break;
		case GLUT_KEY_DOWN:
			speed = -0.1f;
			break;
		}

		player2.setSpeed(speed);
		player2.setDirectionAngleVelocity(directionAngleVelocity);
	}
	else if (gamestate == GAME_MENU) {
		if (key == GLUT_KEY_UP)
			is2player = false;
		else if (key == GLUT_KEY_DOWN)
			is2player = true;
	}
}

void Game::handleSpecialInputUp(int key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		if (!is2player)
			return;

		float speed = player2.getSpeed();
		glm::vec2 directionAngleVelocity = player2.getDirectionAngleVelocity();

		switch (key) {
		case GLUT_KEY_LEFT:
			if (directionAngleVelocity.x > 0)
				directionAngleVelocity.x = 0;
			break;
		case GLUT_KEY_RIGHT:
			if (directionAngleVelocity.x < 0)
				directionAngleVelocity.x = 0;
			break;
		case GLUT_KEY_UP:
			if (speed > 0)
				speed = 0;
			break;
		case GLUT_KEY_DOWN:
			if (speed < 0)
				speed = 0;
			break;
		}

		player2.setSpeed(speed);
		player2.setDirectionAngleVelocity(directionAngleVelocity);
	}
}

void Game::updateBall(int delta) {
	MovableCubeObject* objectsToCollideAgainstBall[2] = { &player1, &player2 };

	ball.move(delta);

	for (size_t i = 0; i < 2; i++) {
		Collision collision = CheckCollision(*objectsToCollideAgainstBall[i], ball);
		if (std::get<0>(collision)) {
			Direction dir = std::get<1>(collision);
			glm::vec2 diffVec = std::get<2>(collision);
			glm::vec3 ballVelocity = ball.getVelocity();
			glm::vec3 ballPosition = ball.getPosition();
			float ballRadius = ball.getRadius();

			if (dir == NEGATIVE_X || dir == POSITIVE_X) {
				ballVelocity.x = -ballVelocity.x;
				float penetration = ballRadius - std::abs(diffVec.x);
				if (dir == NEGATIVE_X)
					ballPosition.x += penetration;
				else
					ballPosition.x -= penetration;
			}
			else {
				ballVelocity.y = -ballVelocity.y;
				float penetration = ball.getRadius() - std::abs(diffVec.y);
				if (dir == NEGATIVE_Y)
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
	player1.move(delta);
	player2.move(delta);

	glm::vec3 player1Position = player1.getPosition();
	glm::vec3 player2Position = player2.getPosition();

	if (player1Position.x + player1.getSize().x > WINDOW_X_SIZE / 2)
		player1Position.x = WINDOW_X_SIZE / 2 - player1.getSize().x;
	if (player2Position.x < WINDOW_X_SIZE / 2)
		player2Position.x = WINDOW_X_SIZE / 2;
	
	player1.setPosition(player1Position);
	player2.setPosition(player2Position);
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
			glm::vec3 velocity(0.0, 0.0, 0.0);

			if (player2.getPosition().y < ball.getPosition().y)
				velocity.y = ball.getVelocity().y * 0.83f;
			else
				velocity.y = -ball.getVelocity().y * 0.83f;

			//player2.setVelocity(velocity);
		}
	}

	if (gamestate == GAME_PLAYING) {
		updateBall(delta);
		updatePlayer(delta);

		if (delayTime > 0)
			delayTime -= delta;

		int epsilon = 2;

		if ((ball.getPosition().x < epsilon) ||
			(ball.getPosition().x + 2 * ball.getRadius() + epsilon > WINDOW_X_SIZE)) {
			if (ball.getPosition().x < epsilon) {
				player1Scored = false;
				score2++;
			}				
			else {
				player1Scored = true;
				score1++;
			}
				

			if (score1 == winningScore || score2 == winningScore) {
				gamestate = GAME_SET;
				std::mt19937 engine((unsigned int)time(NULL));
				std::uniform_int_distribution<int> distribution(0, 1);
				player1Scored = distribution(engine);
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
	else if (gamestate == GAME_SET) {
		updateBall(delta / 2);
	}

	return;
}

void Game::setScreenSize(int width, int height) {
	renderer.setScreenSize(width, height);
}