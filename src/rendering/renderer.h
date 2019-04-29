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
#include "text_renderer.h"
#include "settings.h"
#include "cameraForViewThree.h"

enum ViewMode {
	VIEW_CHARACTER_EYE,
	VIEW_CHARACTER_BACK,
	VIEW_CELLING
};

class Renderer {
private:
	TextRenderer textRenderer;
	Shader shader;
	Model pikachu;
	Model pokeball;
	Model map;
	int width;
	int height;
	
	glm::vec3 getCameraPosition(MovableCubeObject& player1, ViewMode viewmode, CameraForViewThree& cameraForViewThree);
	glm::mat4 getViewMatrix(MovableCubeObject& player1, ViewMode viewmode, CameraForViewThree& cameraForViewThree);
	glm::mat4 makePikachuModelMatrix(MovableCubeObject& player, bool isPlayer1);
	glm::mat4 makePokeballModelMatrix(BallObject& ball);
	glm::mat4 makeMapModelMatrix();
public:
	Renderer() {}
	~Renderer() {}
	void init(int width, int height);
	void setScreenSize(int _width, int _height);
	void renderScene(MovableCubeObject& player1, MovableCubeObject& player2, BallObject& ball, ViewMode viewmode, CameraForViewThree& cameraForViewThree);
	void renderText(ViewMode viewmode, GameState gamestate, int score1, int score2, int delayTime);
};