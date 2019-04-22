#include "renderer.h"
#include "settings.h"

void Renderer::init() {
	shader = Shader("./src/shader.vert", "./src/shader.frag");
	pikachu = Model("./resources/models/pikachu/Pikachu.obj");
	pokeball = Model("./resources/models/pokeball/Pokeball.obj");
	map = Model("./resources/models/box/box.obj");

	glm::vec3 size = map.getMin();
	std::cout << size.x << ' ' << size.y << std::endl;
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

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 view = glm::lookAt(player1.getPosition() + player1.getSize() / 2.0f - player1.getSize().x * 2 * player1.getDirectionVector(),
		player1.getPosition() + player1.getSize() / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float player1DirectionAngle = player1.getDirectionAngle().x;
	float player2DirectionAngle = player2.getDirectionAngle().x;
	   

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, player1.getPosition());
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, player1DirectionAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, -pikachu.getMin());
	model = glm::scale(model, player1.getSize() / pikachu.getSize());
	shader.setMat4("model", model);

	pikachu.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, player2.getPosition());
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, player2DirectionAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, -pikachu.getMin());
	model = glm::scale(model, player2.getSize() / pikachu.getSize());
	shader.setMat4("model", model);

	pikachu.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, ball.getPosition());
	model = glm::scale(model, glm::vec3(ball.getRadius() * 2) / pokeball.getSize());
	model = glm::translate(model, -pokeball.getMin());
	shader.setMat4("model", model);

	pokeball.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, MAP_SIZE / map.getSize());
	model = glm::translate(model, -map.getMin());
	shader.setMat4("model", model);

	map.Draw(shader);
}