#pragma once
#include <glm/vec2.hpp>
#include "game.h"

Collision CheckCollision(CubeObject	cube, BallObject ball);
Collision CheckCollision(CubeObject fixedCube, MovableCubeObject movableCube); 