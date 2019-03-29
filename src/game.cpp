#include "game.h"
#include "render.h"
#include "collision.h"
#include "sceneGraphNode.h"
#include <random>
#include <ctime>
#include <glm/gtc/matrix_transform.hpp>

const float BALLSPEED = 0.15f;
const int DURATION_OF_VIBRATION = 500;

void Game::exit() {
	exiting = true;
}

bool Game::isExiting() {
	return exiting;
}

SceneGraphNode *Game::constructSceneGraph() {
	glm::vec2 player1pos = player1.getPosition();
	glm::vec2 player1size = player1.getSize();
	glm::mat4 backgroundToPikachu1 = glm::translate(glm::mat4(1), glm::vec3(player1pos.x, player1pos.y, 0.0f));
	glm::mat4 pikachuToEar1 =
		glm::translate(glm::mat4(1), glm::vec3(player1size.x / 5, player1size.y * 6 / 7, 0.0)) * 
		glm::rotate(glm::mat4(1), 50.0f, glm::vec3(0.0, 0.0, 1.0));

	return new SceneGraphNode(glm::mat4(1), renderBackground, new SceneGraphNode(
		backgroundToPikachu1, renderPikachu, new SceneGraphNode(
			pikachuToEar1, renderPikachuEar, nullptr, nullptr
		), nullptr
	), nullptr);
}

void Game::render() {
	if (ballCameraMode)
		setBallCamera(ball.getPosition(), ball.getRadius());
	else
		setNormalCamera();

	if (gamestate == GAME_MENU) {
		renderMenu(is2player);
	}
	else {
		SceneGraphNode *root = constructSceneGraph();
		root->traverse();

		renderPikachuOld(player2, false);
		renderNet(net);
		renderBall(ball);
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

glm::vec2 generateUnitVector() {
	float x = 1;
	float y = 0;
	while (x < 0.4 || y < 0.2) {
		x = generateRandomFromZeroToOne();
		y = sqrt(1 - x * x);
	}

	return glm::vec2(x, y);
}

Game::Game() :
	player1(glm::vec2(24.0f, 0.0f), glm::vec2(20.0f, 35.0f), glm::vec2(0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.1f, -0.1f, 0.1f, 0.0),
	player2(glm::vec2(148.0f, 0.0f), glm::vec2(20.0f, 35.0f), glm::vec2(0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.1f, -0.1f, 0.1f, 0.0),
	ball(glm::vec2(88.5f, 70.0f), 7.5f, BALLSPEED * generateUnitVector(), 0.0f, 0.1f),
	net(glm::vec2(93.5f, 0.0f), glm::vec2(5.0f, 50.0f)),
	gamestate(GAME_MENU),
	ballCameraMode(false),
	delayTime(3000),
	is2player(false),
	score1(0), score2(0), winningScore(5),
	player1Scored(false) {
	clouds.push_back(CloudObject(glm::vec2(24.0f, 70.0f), 7.0f, glm::vec2(0.05f, 0.0f), 0.0f, 0.1f));
	clouds.push_back(CloudObject(glm::vec2(148.0f, 70.0f), 7.0f, glm::vec2(0.05f, 0.0f), 0.0f, 0.1f));
}

Game::~Game() {
	std::vector<CloudObject>().swap(clouds);
}

void Game::resetPosition() {
	delayTime = 3000;

	player1.setPosition(glm::vec2(24, 0));
	player1.setsize(glm::vec2(20, 35));
	player1.setVelocity(glm::vec2(0, 0));

	player2.setPosition(glm::vec2(148, 0));
	player2.setsize(glm::vec2(20, 35));
	player2.setVelocity(glm::vec2(0, 0));

	ball.setPosition(glm::vec2(88.5, 70));
	ball.setRadius(7.5);
	glm::vec2 ballVelocity = BALLSPEED * generateUnitVector();
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
			velocity.x = -0.1f;
			break;
		case 'd':
			velocity.x = 0.1f;
			break;
		case 'r':
			restartGame();
			break;
		case ' ':
			ballCameraMode = !ballCameraMode;
			return;
		}

		player1.setVelocity(velocity);
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
			velocity.x = -0.1f;
			break;
		case GLUT_KEY_RIGHT:
			velocity.x = 0.1f;
			break;
		}

		player2.setVelocity(velocity);
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

void Game::updateBall(int delta) {
	RectangleObject* objectsToCollideAgainstBall[3] = { &player1, &player2, &net };

	ball.move(delta);

	for (size_t i = 0; i < 3; i++) {
		Collision collision = CheckCollision(*objectsToCollideAgainstBall[i], ball);
		if (std::get<0>(collision)) {
			if (objectsToCollideAgainstBall[i] == &player1)
				player1.setDurationOfVibration(DURATION_OF_VIBRATION);

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

	if (ballPosition.y < player2Size.y - ballRadius) {
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

void Game::updateClouds(int delta) {
	std::vector<CloudObject>::iterator iter;
	for (iter = clouds.begin(); iter != clouds.end(); iter++)
		iter->move(delta);
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
				velocity.x = ball.getVelocity().x * 0.83f;
			else
				velocity.x = -ball.getVelocity().x * 0.83f;

			player2.setVelocity(velocity);
		}
	}

	if (gamestate == GAME_PLAYING) {
		updateBall(delta);
		updatePlayer(delta);
		updateClouds(delta);

		if (delayTime > 0)
			delayTime -= delta;

		int epsilon = 2;

		if (ball.getPosition().y < epsilon) {
			if (ball.getPosition().x < 92) {
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
		updateClouds(delta/2);
		delayTime -= delta;

		if (delayTime < 0) {
			delayTime = 3000;
			gamestate = GAME_READY;
			resetPosition();
		}
	}
	else if (gamestate == GAME_SET) {
		updateBall(delta / 2);
		updateClouds(delta / 2);
	}

	return;
}