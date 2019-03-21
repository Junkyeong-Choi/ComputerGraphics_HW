#pragma once
#include "object.h"

class BallObject : public Object {
private:
	float radius;
	float electricityAngle;
	glm::vec2 velocity;
public:
	BallObject() {}
	~BallObject() {}
	BallObject(glm::vec2 _position, float _radius, glm::vec2 _velocity) : Object(_position), radius(_radius), velocity(_velocity) {}
	float getRadius() { return radius; }
	float getElectricityAngle() { return electricityAngle; }
	glm::vec2 getVelocity() { return velocity; }
	void setRadius(float _radius) { radius = _radius; }
	void setElectricityAngle(float _electricityAngle) { electricityAngle = _electricityAngle; }
	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
	void move(int delta);
};