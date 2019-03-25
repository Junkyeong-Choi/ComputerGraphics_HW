#pragma once
#include "movableRectangleObject.h"

class CloudObject : public MovableRectangleObject {
private:
	float curling;
public:
	CloudObject() {}
	~CloudObject() {}
	CloudObject(glm::vec2 _position, glm::vec2 _size, glm::vec2 _velocity, float _curling) : MovableRectangleObject(_position, _size, _velocity), curling(_curling) {}
	float getCurling() { return curling; }
	void setCurling(float _curling) { curling = _curling; }
	void move(int delta);
};