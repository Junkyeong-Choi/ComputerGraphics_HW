#pragma once
#include "object.h"

class BallObject : public Object {
private:
	float radius;
	float electricityAngle;
	float electricityAngleSpeed;
	glm::vec2 velocity;
public:
	BallObject() {}
	~BallObject() {}
	BallObject(glm::vec2 _position, float _radius, glm::vec2 _velocity, float _electricityAngle, float _electricityAngleSpeed) :
		Object(_position), radius(_radius), velocity(_velocity), electricityAngle(_electricityAngle), electricityAngleSpeed(_electricityAngleSpeed) {}
	float getRadius() { return radius; }
	float getElectricityAngle() { return electricityAngle; }
	float getElectricityAngleSpeed() { return electricityAngleSpeed; }
	glm::vec2 getVelocity() { return velocity; }
	void setRadius(float _radius) { radius = _radius; }
	void setElectricityAngle(float _electricityAngle) { electricityAngle = _electricityAngle; }
	void setElectricityAngleSpeed(float _electricityAngleSpeed) { electricityAngleSpeed = _electricityAngleSpeed; }
	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
	void move(int delta);
};