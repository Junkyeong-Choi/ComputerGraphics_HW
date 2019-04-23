#pragma once
#include <glm/vec2.hpp>

class CameraForViewThree {
private:
	glm::vec2 velocity;
	glm::vec2 position;
public:
	CameraForViewThree() {}
	~CameraForViewThree() {}
	CameraForViewThree(glm::vec2 _velocity, glm::vec2 _position) : velocity(_velocity), position(_position) {}

	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
	void setPosition(glm::vec2 _position) { position = _position; }

	glm::vec2 getVelocity() { return velocity; }
	glm::vec2 getPosition() { return position; }

	void move(int delta) {
		glm::vec2 displacement = (float)delta * velocity;

		position += displacement;
	}
};