#include "characterObject.h"

void CharacterObject::move(int delta) {
	glm::vec2 displacement = velocity * (float)delta;
	position += displacement;

	if (position.x < 0)
		position.x = 0;
	if (position.x + size.x > 192)
		position.x = 192 - size.x;

	tailProximalAngle += tailProximalAngleSpeed * delta;
	tailDistalAngle += tailDistalAngleSpeed * delta;
	earAngle += earAngleSpeed * delta;

	if (tailProximalAngle > 360)
		tailProximalAngle -= 360;
	if (tailDistalAngle > 360)
		tailDistalAngle -= 360;
	if (earAngle > 360)
		earAngle -= 360;
}