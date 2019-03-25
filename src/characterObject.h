#pragma once
#include "movableRectangleObject.h"

class CharacterObject : public MovableRectangleObject {
private:
	float tailProximalAngle;
	float tailDistalAngle;
	float earAngle;
	float tailProximalAngleSpeed;
	float tailDistalAngleSpeed;
	float earAngleSpeed;
public:
	CharacterObject() {}
	~CharacterObject() {}
	CharacterObject(glm::vec2 _position, glm::vec2 _size, glm::vec2 _velocity, float _tailProximalAngle, float _tailDistalAngle, float _earAngle,
		float _tailProximalAngleSpeed, float _tailDistalAngleSpeed, float _earAngleSpeed)
		: MovableRectangleObject(_position, _size, _velocity), tailProximalAngle(_tailProximalAngle), tailDistalAngle(_tailDistalAngle), earAngle(_earAngle),
		tailProximalAngleSpeed(_tailProximalAngleSpeed), tailDistalAngleSpeed(_tailDistalAngleSpeed), earAngleSpeed(_earAngleSpeed) {}
	float getTailProximalAngle() { return tailProximalAngle; }
	float getTailDistalAngle() { return tailDistalAngle; }
	float getEarAngle() { return earAngle; }
	void setTailProximalAngle(float _tailProximalAngle) { tailProximalAngle = _tailProximalAngle; }
	void setTailDistalAngle(float _tailDistalAngle) { tailDistalAngle = _tailDistalAngle; }
	void setEarAngle(float _earAngle) { earAngle = _earAngle; }
	void move(int delta);
};