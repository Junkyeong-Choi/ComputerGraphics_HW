#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <tuple>
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include "gameObjects.h"

Game game;


void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_FLAT);
	
	game.gameInit();
}


enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
typedef std::tuple<bool, Direction, glm::vec2> Collision;


Direction VectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	float max = 0.0f;
	int match = -1;
	for (int i = 0; i < 4; i++) {
		float dot_product = glm::dot(target, compass[i]);
		if (dot_product > max) {
			max = dot_product;
			match = i;
		}
	}
	return (Direction)match;
}

Collision CheckCollision(RectangleObject rect, BallObject ball) {
	glm::vec2 center(ball.getPosition() + ball.getRadius());

	glm::vec2 aabb_half_extents(rect.getSize() / 2.0f);
	glm::vec2 aabb_center(rect.getPosition().x + aabb_half_extents.x, rect.getPosition().y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;
	difference = closest - center;

	if (glm::length(difference) < ball.getRadius())
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0, 0));
}

Collision CheckCollision(RectangleObject fixed_rect, MovableRectangleObject movable_rect) {

	glm::vec2 fixed_center(fixed_rect.getPosition() + fixed_rect.getSize() / 2.0f);
	glm::vec2 movable_center(movable_rect.getPosition() + movable_rect.getSize() / 2.0f);
	glm::vec2 fixed_half_extents(fixed_rect.getSize() / 2.0f);

	glm::vec2 difference = movable_center - fixed_center;
	glm::vec2 clamped = glm::clamp(difference, -fixed_half_extents, fixed_half_extents);
	glm::vec2 closest = fixed_center + clamped;
	difference = closest - movable_center;

	bool collisionX = fixed_rect.getPosition().x + fixed_rect.getSize().x >= movable_rect.getPosition().x &&
		movable_rect.getPosition().x + movable_rect.getSize().x >= fixed_rect.getPosition().x;
	bool collisionY = fixed_rect.getPosition().y + fixed_rect.getSize().y >= movable_rect.getPosition().y &&
		movable_rect.getPosition().y + movable_rect.getSize().y >= fixed_rect.getPosition().y;

	if (collisionX && collisionY)
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0, 0));
}


void moveObjects() {

}


void renderScene() {

}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(960, 580);
	glutCreateWindow(argv[0]);
	init();
}