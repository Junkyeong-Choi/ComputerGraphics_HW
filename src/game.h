#pragma once
#include <tuple>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "movableRectangleObject.h"
#include "ballObject.h"

enum GameState {
	GAME_MENU,
	GAME_READY,
	GAME_PLAYING,
	GAME_ONE_WIN,
	GAME_TWO_WIN
};


enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
typedef std::tuple<bool, Direction, glm::vec2> Collision;

#define PLAYER_MAX_VELOCITY 0.1

class Game {
private:
	MovableRectangleObject player1;
	MovableRectangleObject player2;
	BallObject ball;
	RectangleObject net;
	GameState gamestate;
	bool ballCameraMode;
	int score1;
	int score2;
	int winningScore;
	bool exiting;

	Direction vectorDirection(glm::vec2 target);
	Collision CheckCollision(RectangleObject rect, BallObject ball);
	Collision CheckCollision(RectangleObject fixed_rect, MovableRectangleObject movable_rect);
	void updateBall(int delta);
	void updatePlayer(int delta);
public:
	Game();
	~Game() {}
	void init(int argc, char* argv[], int width, int height, bool isFullScreen);
	void exit();
	bool isExiting();
	void handleInput(unsigned char key);
	void handleInputUp(unsigned char key);
	void handleSpecialInput(int key);
	void handleSpecialInputUp(int key);
	void update(int delta);
	void render();
};
