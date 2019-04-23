#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include "model.h"
#include "movableCubeObject.h"
#include "ballObject.h"

enum ViewMode {
	VIEW_CHARACTER_EYE,
	VIEW_CHARACTER_BACK,
	VIEW_CELLING
};

class Renderer {
private:
	Shader shader;
	Model pikachu;
	Model pokeball;
	Model map;
	int width;
	int height;
public:
	Renderer() {}
	~Renderer() {}
	void init();
	void setScreenSize(int _width, int _height);
	void render(MovableCubeObject& player1, MovableCubeObject& player2, BallObject& ball);
};