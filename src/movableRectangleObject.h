#pragma once
#include "rectangleObject.h"

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