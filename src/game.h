#pragma once
#include <tuple>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "ballObject.h"
#include "characterObject.h"
#include "cloudObject.h"


enum GameState {
	GAME_MENU,
	GAME_READY,
	GAME_PLAYING,
	GAME_SCORE,
	GAME_SET,
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
	CharacterObject player1;
	CharacterObject player2;
	BallObject ball;
	RectangleObject net;
	std::vector<CloudObject> clouds;
	GameState gamestate;
	bool ballCameraMode;
	bool is2player;
	int delayTime;
	int score1;
	int score2;
	int winningScore;
	bool player1Scored;
	bool exiting;

	void resetPosition();
	void updateBall(int delta);
	void updatePlayer(int delta);
	void updateClouds(int delta);
public:
	Game();
	~Game();
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
