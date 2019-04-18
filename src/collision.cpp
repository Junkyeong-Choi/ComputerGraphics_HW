#include "collision.h" 
#include <algorithm>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

Direction vectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(-1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f)
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

Collision CheckCollision(CubeObject cube, BallObject ball) {

	glm::vec2 ballPositionProjection = glm::vec2(ball.getPosition().x, ball.getPosition().y);
	glm::vec2 cubePositionProjection = glm::vec2(cube.getPosition().x, cube.getPosition().y);
	glm::vec2 cubeSizeProjection = glm::vec2(cube.getSize().x, cube.getSize().y);

	glm::vec2 center(ballPositionProjection + ball.getRadius());

	glm::vec2 aabbHalfExtents(cubeSizeProjection / 2.0f);
	glm::vec2 aabbCenter(cubePositionProjection.x + aabbHalfExtents.x, cubePositionProjection.y + aabbHalfExtents.y);

	glm::vec2 difference = center - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);

	glm::vec2 closest = aabbCenter + clamped;
	difference = closest - center;

	if (glm::length(difference) < ball.getRadius())
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, POSITIVE_X, glm::vec2(0, 0));
}

Collision CheckCollision(CubeObject fixedCube, MovableCubeObject movableCube) {

	glm::vec2 fixedCubePositionProjection = glm::vec2(fixedCube.getPosition().x, fixedCube.getPosition().y);
	glm::vec2 fixedCubeSizeProjection = glm::vec2(fixedCube.getSize().x, fixedCube.getSize().y);
	glm::vec2 movableCubePositionProjection = glm::vec2(movableCube.getPosition().x, movableCube.getPosition().y);
	glm::vec2 movableCubeSizeProjection = glm::vec2(movableCube.getSize().x, movableCube.getSize().y);

	bool collisionX = fixedCubePositionProjection.x + fixedCubeSizeProjection.x >= movableCubePositionProjection.x &&
		movableCubePositionProjection.x + movableCubeSizeProjection.x >= fixedCubePositionProjection.x;
	bool collisionY = fixedCubePositionProjection.y + fixedCubeSizeProjection.y >= movableCubePositionProjection.y &&
		movableCubePositionProjection.y + movableCubeSizeProjection.y >= fixedCubePositionProjection.y;

	if (collisionX && collisionY) {
		float collidingBox_x = std::min({ fixedCubePositionProjection.x + fixedCubeSizeProjection.x - movableCubePositionProjection.x,
		movableCubePositionProjection.x + movableCubeSizeProjection.x - fixedCubePositionProjection.x,
		fixedCubeSizeProjection.x, movableCubeSizeProjection.x });

		float collidingBox_y = std::min({ fixedCubePositionProjection.y + fixedCubeSizeProjection.y - movableCubePositionProjection.y,
			movableCubePositionProjection.y + movableCubeSizeProjection.y - fixedCubePositionProjection.y,
			fixedCubeSizeProjection.y, movableCubeSizeProjection.y });

		Direction direction;
		if (collidingBox_x < collidingBox_y) {
			if (fixedCubePositionProjection.x > movableCubePositionProjection.x)
				direction = POSITIVE_X;
			else
				direction = NEGATIVE_X;
		}
		else {
			if (fixedCubePositionProjection.y > movableCubePositionProjection.y)
				direction = POSITIVE_Y;
			else
				direction = NEGATIVE_Y;
		}
		return std::make_tuple(true, direction, glm::vec2(collidingBox_x, collidingBox_y));
	}

	return std::make_tuple(false, POSITIVE_X, glm::vec2(0, 0));
}
