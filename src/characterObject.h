#pragma once
#include "movableRectangleObject.h"

class CharacterObject : public MovableRectangleObject {
private:
	float tailProximalAngle;
	float tailDistalAngle;
	float earAngle;
	float tailProximalAngleVelocity;
	float tailDistalAngleVelocity;
	float earAngleVelocity;
	int durationOfVibration;
public:
	CharacterObject() {}
	~CharacterObject() {}
	CharacterObject(glm::vec2 _position, glm::vec2 _size, glm::vec2 _velocity, float _tailProximalAngle, float _tailDistalAngle, float _earAngle,
		float _tailProximalAngleVelocity, float _tailDistalAngleVelocity, float _earAngleVelocity, int _durationOfVibration)
		: MovableRectangleObject(_position, _size, _velocity), tailProximalAngle(_tailProximalAngle), tailDistalAngle(_tailDistalAngle), earAngle(_earAngle),
		tailProximalAngleVelocity(_tailProximalAngleVelocity), tailDistalAngleVelocity(_tailDistalAngleVelocity), earAngleVelocity(_earAngleVelocity), durationOfVibration(_durationOfVibration) {}

	float getTailProximalAngle() { return tailProximalAngle; }
	float getTailDistalAngle() { return tailDistalAngle; }
	float getEarAngle() { return earAngle; }
	float getTailProximalAngleVelocity() { return tailProximalAngleVelocity; }
	float getTailDistalAngleVelocity() { return tailDistalAngleVelocity; }
	float getEarAngleVelocity() { return earAngleVelocity; }
	int getDurationOfVibration() { return durationOfVibration; }

	void setTailProximalAngle(float _tailProximalAngle) { tailProximalAngle = _tailProximalAngle; }
	void setTailDistalAngle(float _tailDistalAngle) { tailDistalAngle = _tailDistalAngle; }
	void setEarAngle(float _earAngle) { earAngle = _earAngle; }
	void setTailProximalAngleSpped(float _tailProximalAngleVelocity) { tailProximalAngleVelocity = _tailProximalAngleVelocity; }
	void setTailDistalAngleVelocity(float _tailDistalAngleVelocity) { tailDistalAngleVelocity = _tailDistalAngleVelocity; }
	void setEarAngleVelocity(float _earAngleVelocity) { earAngleVelocity = _earAngleVelocity; }
	void setDurationOfVibration(int _durationOfVibration) { durationOfVibration = _durationOfVibration; }

	void handleCollision(int delta);
	void move(int delta);
};