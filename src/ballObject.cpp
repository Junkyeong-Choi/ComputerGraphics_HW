#include "ballObject.h"

void BallObject::move(int delta) {

	glm::vec2 displacement = velocity * (float)delta;
	position += displacement;

	if (position.x < 0 || position.x + 2 * radius > 192) {
		velocity.x = -velocity.x;
		if (position.x < 0)
			position.x = 0;
		else
			position.x = 192 - 2 * radius;
	}
	if (position.y < 0 || position.y + 2 * radius > 108) {
		velocity.y = -velocity.y;
		if (position.y < 0)
			position.y = 0;
		else
			position.y = 108 - 2 * radius;
	}
}