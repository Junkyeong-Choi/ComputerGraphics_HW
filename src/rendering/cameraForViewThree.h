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

		if (position.x > MAP_SIZE.x / 2)
			position.x = MAP_SIZE.x / 2;
		if (position.x < -MAP_SIZE.x / 5)
			position.x = -MAP_SIZE.x / 5;
		if (position.y > 1.5 * MAP_SIZE.y)
			position.y = 1.5 * MAP_SIZE.y;
		if (position.y < -MAP_SIZE.y / 2)
			position.y = -MAP_SIZE.y / 2;
	}
};