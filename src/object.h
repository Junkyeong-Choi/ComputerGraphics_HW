#pragma once
#include <glm/vec2.hpp>

class Object {
protected:
	glm::vec2 position;
public:
	Object() {}
	~Object() {}
	Object(glm::vec2 _position) : position(_position) {}
	glm::vec2 getPosition() { return position; }
	void setPosition(glm::vec2 _position) { position = _position; }
};
