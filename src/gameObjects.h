#include <glm/vec2.hpp>

class Object {
private:
	glm::vec2 position;
public:
	Object() {}
	~Object() {}
	Object(glm::vec2 _position) : position(_position){}
	glm::vec2 getPosition() { return position; }
	void setPosition(glm::vec2 _position) { position = _position; }
};

class RectangleObject : public Object {
private:
	glm::vec2 size;
public:
	RectangleObject() {}
	~RectangleObject() {}
	RectangleObject(glm::vec2 _position, glm::vec2 _size) : Object(_position), size(_size) {}
	glm::vec2 getSize() { return size; }
	void setsize(glm::vec2 _size) { size = _size; }
};


class MovableRectangleObject : public RectangleObject {
private:
	glm::vec2 velocity;
public:
	MovableRectangleObject() {}
	~MovableRectangleObject() {}
	MovableRectangleObject(glm::vec2 _position, glm::vec2 _size, glm::vec2 _velocity) : RectangleObject(_position, _size), velocity(_velocity) {}
	glm::vec2 getVelocity() { return velocity; }
	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
	void move(int delta);
};

class BallObject : public Object {
private:
	float radius;
	glm::vec2 velocity;
public:
	BallObject() {}
	~BallObject() {}
	BallObject(glm::vec2 _position, float _radius, glm::vec2 _velocity) : Object(_position), radius(_radius), velocity(_velocity) {}
	float getRadius() { return radius; }
	glm::vec2 getVelocity() { return velocity; }
	void setRadius(float _radius) { radius = _radius; }
	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
	void move(int delta);
};

enum GameState {
	GAME_MENU,
	GAME_READY,
	GAME_PLAYING
};

class Game {
private:
	MovableRectangleObject player1;
	MovableRectangleObject player2;
	BallObject ball;
	RectangleObject net;
	GameState gamestate;
public:
	Game();
	~Game() {}
	bool isExiting();
	void update(int delta);
	void render();
};

bool Game::isExiting() {
	return false;
}

void Game::update(int delta) {
	return;
}

void Game::render() {
	return;
}

Game::Game() {
	player1 = MovableRectangleObject(glm::vec2(24, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	player2 = MovableRectangleObject(glm::vec2(148, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	ball = BallObject(glm::vec2(90, 70), 7.5, glm::vec2(0,0));
	net = RectangleObject(glm::vec2(90, 0), glm::vec2(5, 50));
	gamestate = GAME_MENU;
}