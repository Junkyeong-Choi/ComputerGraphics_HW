#include "characterObject.h"


const int TAIL_PROXIMAL_ANGLE_LIMIT = 15;
const int TAIL_DISTAL_ANGLE_LIMIT = 15;
const int EAR_ANGLE_LIMIT = 15;

void CharacterObject::move(int delta) {
	glm::vec2 displacement = velocity * (float)delta;
	position += displacement;

	if (position.x < 0)
		position.x = 0;
	if (position.x + size.x > 192)
		position.x = 192 - size.x;

	if (tailProximalAngle > 360)
		tailProximalAngle -= 360;
	if (tailDistalAngle > 360)
		tailDistalAngle -= 360;
	if (earAngle > 360)
		earAngle -= 360;

	durationOfVibration -= delta;
	if (durationOfVibration > 0) {
		tailProximalAngle += tailProximalAngleVelocity * delta;
		tailDistalAngle += tailDistalAngleVelocity * delta;
		earAngle += earAngleVelocity * delta;

		if (tailProximalAngle > TAIL_PROXIMAL_ANGLE_LIMIT || tailProximalAngle < -TAIL_PROXIMAL_ANGLE_LIMIT)
			tailProximalAngleVelocity = -tailProximalAngleVelocity;
		if (tailDistalAngle > TAIL_DISTAL_ANGLE_LIMIT || tailDistalAngle < -TAIL_DISTAL_ANGLE_LIMIT)
			tailDistalAngleVelocity = -tailDistalAngleVelocity;
		if (earAngle > EAR_ANGLE_LIMIT || earAngle < -EAR_ANGLE_LIMIT)
			earAngleVelocity = -earAngleVelocity;
	}
	else
		durationOfVibration = 0;
}