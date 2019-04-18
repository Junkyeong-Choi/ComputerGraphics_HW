#pragma once
#include "object.h"

class BallObject : public Object {
private:
	float radius;
	glm::vec3 velocity;
public:
	BallObject() {}
	~BallObject() {}
	BallObject(glm::vec3 _position, float _radius, glm::vec3 _velocity) :
		Object(_position), radius(_radius), velocity(_velocity) {}
	float getRadius() { return radius; }
	glm::vec3 getVelocity() { return velocity; }
	void setRadius(float _radius) { radius = _radius; }
	void setVelocity(glm::vec3 _velocity) { velocity = _velocity; }
	void move(int delta);
};