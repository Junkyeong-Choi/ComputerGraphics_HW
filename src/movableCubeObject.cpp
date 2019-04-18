#include "movableCubeObject.h"

void MovableCubeObject::move(int delta) {

	glm::vec3 displacement = velocity * (float)delta;
	position += displacement;
		
	if (position.x < 0)
		position.x = 0;
	if (position.x + size.x > 192)
		position.x = 192 - size.x;
	if (position.y < 0)
		position.y = 0;
	if (position.y + size.y > 108)
		position.y = 108 - size.y;
}