#include "collision.h" 
#include <algorithm>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

Direction vectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	float max = 0.0f;
	int match = -1;
	for (int i = 0; i < 4; i++) {
		float dotProduct = glm::dot(target, compass[i]);
		if (dotProduct > max) {
			max = dotProduct;
			match = i;
		}
	}
	return (Direction)match;
}

Collision CheckCollision(RectangleObject rect, BallObject ball) {
	glm::vec2 center(ball.getPosition() + ball.getRadius());

	glm::vec2 aabbHalfExtents(rect.getSize() / 2.0f);
	glm::vec2 aabbCenter(rect.getPosition().x + aabbHalfExtents.x, rect.getPosition().y + aabbHalfExtents.y);

	glm::vec2 difference = center - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);

	glm::vec2 closest = aabbCenter + clamped;
	difference = closest - center;

	if (glm::length(difference) < ball.getRadius())
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0, 0));
}

Collision CheckCollision(RectangleObject fixedRect, MovableRectangleObject movableRect) {

	glm::vec2 fixedRectPosition = fixedRect.getPosition();
	glm::vec2 fixedRectSize = fixedRect.getSize();
	glm::vec2 movableRectPosition = movableRect.getPosition();
	glm::vec2 movableRectSize = movableRect.getSize();

	bool collisionX = fixedRectPosition.x + fixedRectSize.x >= movableRectPosition.x &&
		movableRectPosition.x + movableRectSize.x >= fixedRectPosition.x;
	bool collisionY = fixedRectPosition.y + fixedRectSize.y >= movableRectPosition.y &&
		movableRectPosition.y + movableRectSize.y >= fixedRectPosition.y;

	if (collisionX && collisionY) {
		float collidingBox_x = std::min({ fixedRectPosition.x + fixedRectSize.x - movableRectPosition.x,
		movableRectPosition.x + movableRectSize.x - fixedRectPosition.x,
		fixedRectSize.x, movableRectSize.x });

		float collidingBox_y = std::min({ fixedRectPosition.y + fixedRectSize.y - movableRectPosition.y,
			movableRectPosition.y + movableRectSize.y - fixedRectPosition.y,
			fixedRectSize.y, movableRectSize.y });

		Direction direction;
		if (collidingBox_x < collidingBox_y) {
			if (fixedRectPosition.x > movableRectPosition.x)
				direction = RIGHT;
			else
				direction = LEFT;
		}
		else {
			if (fixedRectPosition.y > movableRectPosition.y)
				direction = UP;
			else
				direction = DOWN;
		}
		return std::make_tuple(true, direction, glm::vec2(collidingBox_x, collidingBox_y));
	}

	return std::make_tuple(false, UP, glm::vec2(0, 0));
}
