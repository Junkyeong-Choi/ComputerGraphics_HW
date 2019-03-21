#pragma once
#include <glm/vec2.hpp>
#include "game.h"

Collision CheckCollision(RectangleObject rect, BallObject ball);
Collision CheckCollision(RectangleObject fixedRect, MovableRectangleObject movableRect); 