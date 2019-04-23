#pragma once
#include "cubeObject.h"
#include "settings.h"

class MovableCubeObject : public CubeObject {
protected:
	float speed;
	glm::vec2 directionAngle;		//consists of (the rotation angle about z-axis (on xy plane), the angle between z-axis and direction vector)
	glm::vec2 directionAngleVelocity;

	void checkAngle() {
		if (directionAngle.x > 2 * PI)
			directionAngle.x -= 2 * PI;
		if (directionAngle.x < 0)
			directionAngle.x += 2 * PI;
		if (directionAngle.y > 2 * PI)
			directionAngle.y -= 2 * PI;
		if (directionAngle.y < 0)
			directionAngle.y += 2 * PI;
	}
public:
	MovableCubeObject() {}
	~MovableCubeObject() {}
	MovableCubeObject(glm::vec3 _position, glm::vec3 _size, float _speed, glm::vec2 _directionAngle, glm::vec2 _directionAngleVelocity) : 
		CubeObject(_position, _size), speed(_speed), directionAngle(_directionAngle), directionAngleVelocity(_directionAngleVelocity) {}
	float getSpeed() { return speed; }
	glm::vec2 getDirectionAngle() { return directionAngle; }
	glm::vec2 getDirectionAngleVelocity() { return directionAngleVelocity; }
	glm::vec3 getDirectionVector() {
		float theta = directionAngle.x;			// the rotation angle about z-axis (on xy plane)
		float phi = directionAngle.y;			// the angle between z-axis and direction vector

		return glm::vec3(cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi));
	}
	glm::vec3 getVelocity() { return speed * getDirectionVector(); }

	void setSpeed(float _speed) { speed = _speed; }
	void setDirectionAngle(glm::vec2 _directionAngle) { directionAngle = _directionAngle; }
	void setDirectionAngleVelocity(glm::vec2 _directionAngleVelocity) { directionAngleVelocity = _directionAngleVelocity; }
	void setVelocity(glm::vec3 _velocity) {
		speed = glm::length(_velocity);
		glm::vec3 normalized = _velocity / speed;
		float phi = acos(normalized.z);			//phi is in [0, PI]
		float theta = atan2(_velocity.y, _velocity.x);
		directionAngle = glm::vec2(theta, phi);
	}

	void move(int delta) {
		glm::vec3 displacement = getVelocity() * (float)delta;
		position += displacement;

		if (position.x < 0)
			position.x = 0;
		if (position.x + size.x > MAP_SIZE.x)
			position.x = MAP_SIZE.x - size.x;
		if (position.y < 0)
			position.y = 0;
		if (position.y + size.y > MAP_SIZE.y)
			position.y = MAP_SIZE.y - size.y;

		glm::vec2 angleDisplacement = directionAngleVelocity * (float)delta;
		directionAngle += angleDisplacement;

		checkAngle();
	}
};