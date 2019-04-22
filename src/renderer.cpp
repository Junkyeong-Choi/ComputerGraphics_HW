#include "renderer.h"
#include "settings.h"

void Renderer::init() {
	shader = Shader("./src/shader.vert", "./src/shader.frag");
	pikachu = Model("./resources/models/pikachu/Pikachu.obj");
	pokeball = Model("./resources/models/pokeball/Pokeball.obj");
	map = Model("./resources/models/box/box.obj");
}

void Renderer::setScreenSize(int _width, int _height) {
	width = _width;
	height = _height;
}

void Renderer::render(MovableCubeObject& player1, MovableCubeObject& player2, BallObject& ball) {
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	shader.use();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(-50.0f, 0.0f, 0.0f) + player1.getPosition(),
		glm::vec3(0.0f, 0.0f, 0.0f) + player1.getPosition(), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, player1.getPosition());
	model = glm::scale(model, player1.getSize() / pikachu.getSize());
	model = glm::translate(model, pikachu.getMin());
	shader.setMat4("model", model);

	pikachu.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, player2.getPosition());
	model = glm::scale(model, player2.getSize() / pikachu.getSize());
	model = glm::translate(model, pikachu.getMin());
	shader.setMat4("model", model);

	pikachu.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, ball.getPosition());
	model = glm::scale(model, glm::vec3(ball.getRadius() * 2) / pokeball.getSize());
	model = glm::translate(model, pokeball.getMin());
	shader.setMat4("model", model);

	pokeball.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, MAP_SIZE / map.getSize());
	model = glm::translate(model, map.getMin());
	shader.setMat4("model", model);

	map.Draw(shader);
}