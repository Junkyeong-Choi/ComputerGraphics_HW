#pragma once
#include <glm/geometric.hpp>

const float DEG2RAD = 3.141592f / 180.0f;

void setNormalCamera();
void setBallCamera(glm::vec2 pos, float radius);
void renderScore(int score1, int score2);
void renderReady(int delayTime);
void renderGo();
void renderScoreText();
void renderCameraText(bool ballCameraMode);
void renderMenu(bool is2player);
void renderWinText(bool is2player, int score1, int score2);
void renderBackground(void);
void renderPikachu(void);
void renderPikachuEar(void);
void renderPikachuProximalTail(void);
void renderPikachuOld(MovableRectangleObject player, bool isLeft);
void renderNet(RectangleObject net);
void renderBall(BallObject ball);