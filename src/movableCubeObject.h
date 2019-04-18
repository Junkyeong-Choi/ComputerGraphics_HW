#pragma once
#include "cubeObject.h"

class MovableCubeObject : public CubeObject {
protected:
	glm::vec3 velocity;
public:
	MovableCubeObject() {}
	~MovableCubeObject() {}
	MovableCubeObject(glm::vec3 _position, glm::vec3 _size, glm::vec3 _velocity) : CubeObject(_position, _size), velocity(_velocity) {}
	glm::vec3 getVelocity() { return velocity; }
	void setVelocity(glm::vec3 _velocity) { velocity = _velocity; }
	void move(int delta);
};