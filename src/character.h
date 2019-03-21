#pragma once
#include "movableRectangleObject.h"

class Character : public MovableRectangleObject {
private:
	float tailProximalAngle;
	float tailDistalAngle;
	float earAngle;
public:
	Character() {}
	~Character() {}
	Character(glm::vec2 _position, glm::vec2 _size, glm::vec2 _velocity, float _tailProximalAngle, float _tailDistalAngle, float _earAngle)
		: MovableRectangleObject(_position, _size, _velocity), tailProximalAngle(_tailProximalAngle), tailDistalAngle(_tailDistalAngle), earAngle(_earAngle) {}
	float getTailProximalAngle() { return tailProximalAngle; }
	float getTailDistalAngle() { return tailDistalAngle; }
	float getEarAngle() { return earAngle; }
	void setTailProximalAngle(float _tailProximalAngle) { tailProximalAngle = _tailProximalAngle; }
	void setTailDistalAngle(float _tailDistalAngle) { tailDistalAngle = _tailDistalAngle; }
	void setEarAngle(float _earAngle) { earAngle = _earAngle; }
};