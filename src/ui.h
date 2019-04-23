#pragma once

const float DEG2RAD = 3.141592f / 180.0f;

void drawEllipse(float x, float y, float xradius, float yradius, int startDegree = 0, int endDegree = 360);
void drawEllipseStroke(float x, float y, float xradius, float yradius, int startDegree = 0, int endDegree = 360);
void set2DCamera();
void renderMenuPikachuHalf();
void renderMenu(bool is2player);