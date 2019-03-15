#include "movableRectangleObject.h"

void MovableRectangleObject::move(int delta) {

	glm::vec2 displacement = velocity * (float)delta;
	position += displacement;

	if (position.x < 0)
		position.x = 0;
	if (position.x + size.x > 192)
		position.x = 192 - size.x;

}