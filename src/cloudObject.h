#pragma once
#include "movableRectangleObject.h"

class CloudObject : public MovableRectangleObject {
private:
	float curling;
	float curlingSpeed;
public:
	CloudObject() {}
	~CloudObject() {}
	CloudObject(glm::vec2 _position, glm::vec2 _size, glm::vec2 _velocity, float _curling, float _curlingSpeed) : 
		MovableRectangleObject(_position, _size, _velocity), curling(_curling), curlingSpeed(_curlingSpeed) {}
	float getCurling() { return curling; }
	float getCrulingSpeed() { return curlingSpeed; }
	void setCurling(float _curling) { curling = _curling; }
	void setCurlingSpeed(float _curlingSpeed) { curlingSpeed = _curlingSpeed; }
	void move(int delta);
};