#include "movableCubeObject.h"

void MovableCubeObject::checkAngle() {
	if (directionAngle.x > 2 * PI)
		directionAngle.x -= 2 * PI;
	if (directionAngle.x < 0)
		directionAngle.x += 2 * PI;
	if (directionAngle.y > 2 * PI)
		directionAngle.y -= 2 * PI;
	if (directionAngle.y < 0)
		directionAngle.y += 2 * PI;
}

glm::vec3 MovableCubeObject::getDirectionVector() {
	float theta = directionAngle.x;			// the rotation angle about z-axis (on xy plane)
	float phi = directionAngle.y;			// the angle between z-axis and direction vector

	return glm::vec3(cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi));
}

void MovableCubeObject::move(int delta) {

	glm::vec3 displacement = getVelocity() * (float)delta;
	position += displacement;
		
	if (position.x < 0)
		position.x = 0;
	if (position.x + size.x > 192)
		position.x = 192 - size.x;
	if (position.y < 0)
		position.y = 0;
	if (position.y + size.y > 108)
		position.y = 108 - size.y;

	glm::vec2 angleDisplacement = directionAngleVelocity * (float)delta;
	directionAngle += angleDisplacement;
	
	checkAngle();
}