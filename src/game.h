#pragma once
#include <tuple>
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

class Game {
private:
	MovableRectangleObject player1;
	MovableRectangleObject player2;
	BallObject ball;
	RectangleObject net;
	GameState gamestate;
	Direction vectorDirection(glm::vec2 target);
	Collision CheckCollision(RectangleObject rect, BallObject ball);
	Collision CheckCollision(RectangleObject fixed_rect, MovableRectangleObject movable_rect);
public:
	Game();
	~Game() {}
	bool isExiting();
	void update(int delta);
	void render();
};
