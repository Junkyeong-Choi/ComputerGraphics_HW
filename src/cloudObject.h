#pragma once
#include "movableRectangleObject.h"

class CloudObject : public Object {
private:
	float radius;
	glm::vec2 velocity;
	float curling;
	float curlingVelocity;
public:
	CloudObject() {}
	~CloudObject() {}
	CloudObject(glm::vec2 _position, float _radius, glm::vec2 _velocity, float _curling, float _curlingVelocity) : 
		Object(_position), radius(_radius), curling(_curling), curlingVelocity(_curlingVelocity) {}
	float getRadius() { return radius; }
	glm::vec2 getVelocity() { return velocity; }
	float getCurling() { return curling; }
	float getCrulingVelocity() { return curlingVelocity; }

	void setRadius(float _radius) { radius = _radius; }
	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
	void setCurling(float _curling) { curling = _curling; }
	void setCurlingSpeed(float _curlingVelocity) { curlingVelocity = _curlingVelocity; }
	void move(int delta);
};