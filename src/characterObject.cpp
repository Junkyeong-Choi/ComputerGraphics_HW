#include "characterObject.h"


const float TAIL_PROXIMAL_ANGLE_LIMIT = 15;
const float TAIL_DISTAL_ANGLE_LIMIT = 15;
const float EAR_ANGLE_LIMIT = 15;

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

		float epsilon = 0.05;
		if (tailProximalAngle > TAIL_PROXIMAL_ANGLE_LIMIT || tailProximalAngle < -TAIL_PROXIMAL_ANGLE_LIMIT) {
			tailProximalAngleVelocity = -tailProximalAngleVelocity;
			if (tailProximalAngle > TAIL_PROXIMAL_ANGLE_LIMIT)
				tailProximalAngle = TAIL_PROXIMAL_ANGLE_LIMIT - epsilon;
			else
				tailProximalAngle = -TAIL_PROXIMAL_ANGLE_LIMIT + epsilon;

		}
			if (tailDistalAngle > TAIL_DISTAL_ANGLE_LIMIT || tailDistalAngle < -TAIL_DISTAL_ANGLE_LIMIT) {
				tailDistalAngleVelocity = -tailDistalAngleVelocity;
				if (tailDistalAngle > TAIL_DISTAL_ANGLE_LIMIT)
					tailDistalAngle = TAIL_DISTAL_ANGLE_LIMIT - epsilon;
				else
					tailDistalAngle = -TAIL_DISTAL_ANGLE_LIMIT + epsilon;
		}
			if (earAngle > EAR_ANGLE_LIMIT || earAngle < -EAR_ANGLE_LIMIT) {
				earAngleVelocity = -earAngleVelocity;
				if (earAngle > EAR_ANGLE_LIMIT)
					earAngle = EAR_ANGLE_LIMIT - epsilon;
				else
					earAngle = -EAR_ANGLE_LIMIT + epsilon;
		}
	}

	else
		durationOfVibration = 0;
}