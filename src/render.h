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
void renderPikachuDistalTail(void);
void renderNet(void);
void renderBall(void);
void renderElectricLine(void);