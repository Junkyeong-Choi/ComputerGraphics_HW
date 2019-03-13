#pragma once
#include <glm/geometric.hpp>

void setNormalCamera();
void setBallCamera(glm::vec2 pos, float radius);
void renderScore(int score1, int score2);
void renderReady(int delayTime);
void renderGo();
void renderScoreText();