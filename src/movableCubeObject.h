#pragma once
#include "cubeObject.h"

const float PI = (float)3.14159265358979;

class MovableCubeObject : public CubeObject {
protected:
	float speed;
	glm::vec2 directionAngle;		//consists of (the rotation angle about z-axis (on xy plane), the angle between z-axis and direction vector)
	glm::vec2 directionAngleVelocity;

	void checkAngle();
public:
	MovableCubeObject() {}
	~MovableCubeObject() {}
	MovableCubeObject(glm::vec3 _position, glm::vec3 _size, float _speed, glm::vec2 _directionAngle, glm::vec2 _directionAngleVelocity) : 
		CubeObject(_position, _size), speed(_speed), directionAngle(_directionAngle), directionAngleVelocity(_directionAngleVelocity) {}
	float getSpeed() { return speed; }
	glm::vec2 getDirectionAngle() { return directionAngle; }
	glm::vec2 getDirectionAngleVelocity() { return directionAngleVelocity; }
	glm::vec3 getDirectionVector();
	glm::vec3 getVelocity() { return speed * getDirectionVector(); }

	void setSpeed(float _speed) { speed = _speed; }
	void setDirectionAngle(glm::vec2 _directionAngle) { directionAngle = _directionAngle; }
	void setDirectionAngleVelocity(glm::vec2 _directionAngleVelocity) { directionAngleVelocity = _directionAngleVelocity; }

	void move(int delta);
};