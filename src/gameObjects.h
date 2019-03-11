#include <glm/vec2.hpp>

class Object {
private:
	glm::vec2 size;
	glm::vec2 position;
public:
	glm::vec2 getSize() { return size; }
	glm::vec2 setPosition() { return position; }
	void setSize(glm::vec2 _size) { size = _size; }
	void setPosition(glm::vec2 _position) { position = _position; }
};

class MovableObject : public Object {
private:
	glm::vec2 velocity;
public:
	glm::vec2 getVelocity() { return velocity; }
	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
};

class Player :public MovableObject {
private:
public:
};

class Game {
public:
private:
};
