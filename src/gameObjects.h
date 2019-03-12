#include <glm/vec2.hpp>
#include <iostream>

using namespace std;

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
	Game(int argc, char* argv[], int width, int height, bool isFullScreen);
	~Game() {}
	bool isExiting();
	void update(int delta);
	void render();
};

bool Game::isExiting() {
	return false;
}

void Game::update(int delta) {
	cout << delta << endl;
	return;
}

void Game::render() {
}

void display() {
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

Game::Game(int argc, char* argv[], int width, int height, bool isFullScreen) {
	// Make in-game objects
	player1 = MovableRectangleObject(glm::vec2(24, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	player2 = MovableRectangleObject(glm::vec2(148, 0), glm::vec2(20, 35), glm::vec2(0, 0));
	ball = BallObject(glm::vec2(90, 70), 7.5, glm::vec2(0,0));
	net = RectangleObject(glm::vec2(90, 0), glm::vec2(5, 50));
	gamestate = GAME_MENU;

	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pickachu Volleyball");
	if (isFullScreen)
		glutFullScreen();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
}