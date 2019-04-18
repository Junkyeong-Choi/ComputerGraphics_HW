#pragma once
#include <glm/vec3.hpp>

class Object {
protected:
	glm::vec3 position;
public:
	Object() {}
	~Object() {}
	Object(glm::vec3 _position) : position(_position) {}
	glm::vec3 getPosition() { return position; }
	void setPosition(glm::vec3 _position) { position = _position; }
};
