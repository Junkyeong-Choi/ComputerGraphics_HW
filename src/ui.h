#pragma once

const float DEG2RAD = 3.141592f / 180.0f;

void drawEllipse(float x, float y, float xradius, float yradius, int startDegree = 0, int endDegree = 360);
void drawEllipseStroke(float x, float y, float xradius, float yradius, int startDegree = 0, int endDegree = 360);
void set2DCamera();
int bitmapStringLength(void* font, const char* text);
void renderText(float x, float y, const char* text);
void renderScore(int score1, int score2);
void renderReady(int delayTime);
void renderGo();
void renderScoreText();
void renderCameraText(bool ballCameraMode);
void renderMenuPikachuHalf();
void renderMenu(bool is2player);
void renderWinText(bool is2player, int score1, int score2);