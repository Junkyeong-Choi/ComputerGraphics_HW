#include "game.h"
#include "collision.h"
#include <random>
#include <ctime>
#include <glm/gtc/matrix_transform.hpp>
#include "rendering/renderer.h"
#include "rendering/ui.h"
#include "game.h"
#include "settings.h"

void Game::exit() {
	exiting = true;
}

bool Game::isExiting() {
	return exiting;
}

void Game::render() {
	if (gamestate == GAME_MENU) {
		renderMenu();
	}
	else {
		renderer.renderScene(player1, player2, ball, viewmode, cameraForViewThree);
		renderer.renderText(viewmode, gamestate, score1, score2, delayTime);
	}

	glutSwapBuffers();
}

float generateRandomFromZeroToOne() {
	std::mt19937 engine((unsigned int)time(NULL));
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	return distribution(engine);
}

glm::vec3 generateUnitVector_old() {
	float x = 0;
	float y = 1;
	while (abs(y) > 0.5) {
		y = generateRandomFromZeroToOne();
		y -= 0.5;
		y *= 2;
		x = sqrt(1 - y * y);
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
	delayTime(3000),
	score1(0), score2(0), winningScore(WINNING_SCORE),
	player1Scored(false),
	viewmode(VIEW_CHARACTER_BACK),
	cameraForViewThree(glm::vec2(0.0f,0.0f), glm::vec2(PLAYER_ONE_POSITION)),
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
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pickachu Volleyball");
	if (isFullScreen)
		glutFullScreen();

	glewInit();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, width, height);

	renderer.init(width, height);
}

void Game::handleInput(unsigned char key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		float speed = player1.getSpeed();
		glm::vec2 directionAngleVelocity = player1.getDirectionAngleVelocity();
		glm::vec2 cameraForViewThreeVelocity = cameraForViewThree.getVelocity();
		switch (key) {
		case 'a':
			directionAngleVelocity.x = 0.005f;
			break;
		case 'd':
			directionAngleVelocity.x = -0.005f;
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
		case 'j':
			cameraForViewThreeVelocity.y = 0.1f;
			break;
		case 'l':
			cameraForViewThreeVelocity.y = -0.1f;
			break;
		case 'i':
			cameraForViewThreeVelocity.x = 0.1f;
			break;
		case 'k':
			cameraForViewThreeVelocity.x = -0.1f;
			break;
		}
		player1.setSpeed(speed);
		player1.setDirectionAngleVelocity(directionAngleVelocity);
		cameraForViewThree.setVelocity(cameraForViewThreeVelocity);
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
		viewmode = (ViewMode)((viewmode + 1) % 3);
	else if (key == 'h')
		renderer.setHiddenLineRemoval(!renderer.getHiddenLineRemoval());
}

void Game::handleInputUp(unsigned char key) {
	if (gamestate == GAME_PLAYING || gamestate == GAME_SCORE) {
		float speed = player1.getSpeed();
		glm::vec2 directionAngleVelocity = player1.getDirectionAngleVelocity();
		glm::vec2 cameraForViewThreeVelocity = cameraForViewThree.getVelocity();

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
		case 'j':
			if (cameraForViewThreeVelocity.y > 0)
				cameraForViewThreeVelocity.y = 0;
			break;
		case 'l':
			if (cameraForViewThreeVelocity.y < 0)
				cameraForViewThreeVelocity.y = 0;
			break;
		case 'i':
			if (cameraForViewThreeVelocity.x > 0)
				cameraForViewThreeVelocity.x = 0;
			break;
		case 'k':
			if (cameraForViewThreeVelocity.x < 0)
				cameraForViewThreeVelocity.x = 0;
			break;
		}

		player1.setSpeed(speed);
		player1.setDirectionAngleVelocity(directionAngleVelocity);
		cameraForViewThree.setVelocity(cameraForViewThreeVelocity);
	}

	if (key == 27) // ESC Key
		this->exit();
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

	if (player1Position.x + player1.getSize().x > MAP_SIZE.x / 2)
		player1Position.x = MAP_SIZE.x / 2 - player1.getSize().x;
	if (player2Position.x < MAP_SIZE.x / 2)
		player2Position.x = MAP_SIZE.x / 2;
	
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
		float speed(abs(ball.getVelocity().y) * 0.90f);
		glm::vec3 player2Position = player2.getPosition();
		glm::vec2 player2DirectionAngle = player2.getDirectionAngle();
		glm::vec2 player2DirectionAngleVelocity = player2.getDirectionAngleVelocity();

		float epsilon = 0.003f;

		if (player2Position.y < ball.getPosition().y) {
			if (player2DirectionAngle.x < 1.0f / 2.0f * PI - epsilon)
				player2DirectionAngleVelocity.x = 0.005f;

			else if (player2DirectionAngle.x > 1.0f / 2.0f * PI + epsilon)
				player2DirectionAngleVelocity.x = -0.005f;
			else
				player2DirectionAngleVelocity.x = 0.0f;
		}
		else {
			if (player2DirectionAngle.x < 3.0f / 2.0f * PI - epsilon)
				player2DirectionAngleVelocity.x = 0.005f;

			else
				player2DirectionAngleVelocity.x = 0.0f;
		}

		player2.setSpeed(speed);
		player2.setDirectionAngleVelocity(player2DirectionAngleVelocity);

	}

	if (gamestate == GAME_PLAYING) {
		updateBall(delta);
		updatePlayer(delta);
		cameraForViewThree.move(delta);

		if (delayTime > 0)
			delayTime -= delta;

		int epsilon = 2;

		if ((ball.getPosition().x < epsilon) ||
			(ball.getPosition().x + 2 * ball.getRadius() + epsilon > MAP_SIZE.x)) {
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
		cameraForViewThree.move(delta/2);
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