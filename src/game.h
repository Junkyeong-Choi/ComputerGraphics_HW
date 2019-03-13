#pragma once
#include <tuple>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "movableRectangleObject.h"
#include "ballObject.h"

enum GameState {
	GAME_MENU,
	GAME_READY,
	GAME_PLAYING
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
	Direction vectorDirection(glm::vec2 target);
	Collision CheckCollision(RectangleObject rect, BallObject ball);
	Collision CheckCollision(RectangleObject fixed_rect, MovableRectangleObject movable_rect);
	void updateBall(int delta);
	void updatePlayer(int delta);
	bool exiting;
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
