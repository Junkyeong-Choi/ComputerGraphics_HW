#pragma once
#include <glm/glm.hpp>

const float PI = (float)3.14159265358979;

const float PLAYER_MAX_VELOCITY = 0.1f;

const glm::vec3 PLAYER_ONE_POSITION = glm::vec3(24.0f, 54.0f, 0.0f);
const glm::vec3 PLAYER_ONE_SIZE = glm::vec3(20.0f, 15.0f, 30.0f);
const float PLAYER_ONE_SPEED = 0;
const glm::vec2 PLAYER_ONE_DIRECTION_ANGLE = glm::vec2(0.0f, PI / 2);
const glm::vec2 PLAYER_ONE_DIRECTION_ANGLE_VELOCITY = glm::vec2(0.0f, 0.0f);


const glm::vec3 PLAYER_TWO_POSITION = glm::vec3(148.0f, 54.0f, 0.0f);
const glm::vec3 PLAYER_TWO_SIZE = glm::vec3(20.0f, 15.0f, 30.0f);
const float PLAYER_TWO_SPEED = 0;
const glm::vec2 PLAYER_TWO_DIRECTION_ANGLE = glm::vec2(-PI, PI / 2);
const glm::vec2 PLAYER_TWO_DIRECTION_ANGLE_VELOCITY = glm::vec2(0.0f, 0.0f);


const glm::vec3 BALL_POSITION = glm::vec3(88.5f, 54.0f, 0.0f);
const float BALL_RADIUS = 7.5f;
const float BALL_SPEED = 0.01f;

const glm::vec3 MAP_SIZE = glm::vec3(192.0f, 108.0f, 50.0f);