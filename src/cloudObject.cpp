#include "cloudObject.h"

void CloudObject::move(int delta) {
	const float CLOUDSIZE_RADIUS_RATIO = 4.6;
	const float CLOUD_ARM_LIMIT = 30;
	
	glm::vec2 displacement = velocity * (float)delta;
	position += displacement;

	if (velocity.x < 0 || position.x + CLOUDSIZE_RADIUS_RATIO * radius < 0)
		position.x += 192;
	if (velocity.x > 0 || position.x > 192)
		position.x -= 192;


	curling += curlingVelocity * delta;
	float epsilon = 0.05;
	if (curling > CLOUD_ARM_LIMIT || curling < -CLOUD_ARM_LIMIT) {
		curlingVelocity = -curlingVelocity;
		if (curling > CLOUD_ARM_LIMIT)
			curling = CLOUD_ARM_LIMIT;
		else
			curling = -CLOUD_ARM_LIMIT;
	}
}