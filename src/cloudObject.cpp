#include "cloudObject.h"

void CloudObject::move(int delta) {

	glm::vec2 displacement = velocity * (float)delta;
	position += displacement;

	if (position.x + size.x < 0)
		position.x += 192;
	if (position.x > 192)
		position.x -= 192;

	curling += curlingSpeed * delta;
}