#include "renderer.h"
#include "settings.h"

void Renderer::init(int width, int height) {
	shader = Shader("./src/shader.vert", "./src/shader.frag");
	pikachu = Model("./resources/models/pikachu/Pikachu.obj");
	pokeball = Model("./resources/models/pokeball/pokeball2.obj");
	map = Model("./resources/models/box/box.obj");
	textRenderer = TextRenderer(width, height);
	textRenderer.Load("./resources/fonts/OCRAEXT.TTF", 48);
}

void Renderer::setScreenSize(int _width, int _height) {
	width = _width;
	height = _height;
}

void Renderer::render(MovableCubeObject& player1, MovableCubeObject& player2, BallObject& ball, ViewMode viewmode) {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	
	glm::vec3 eye, center, up;
	if (viewmode == VIEW_CHARACTER_EYE) {
		eye = player1.getPosition() + (player1.getSize() / 2.0f) + (player1.getSize().x / 2.0f) * player1.getDirectionVector();
		center = eye + player1.getDirectionVector();
		up = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (viewmode == VIEW_CHARACTER_BACK) {
		eye = player1.getPosition() + (player1.getSize() / 2.0f) - player1.getSize().x * 2 * player1.getDirectionVector() + glm::vec3(0.0f, 0.0f, 10.0f);
		center = eye + player1.getDirectionVector() + glm::vec3(0.0f, 0.0f, -0.1f);
		up = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (viewmode == VIEW_CELLING) {
		eye = glm::vec3(player1.getPosition().x, player1.getPosition().y, MAP_SIZE.z * 3.0f);
		center = glm::vec3(MAP_SIZE.x / 2.0f, MAP_SIZE.y / 2.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	glm::mat4 view = glm::lookAt(eye, center, up);
		
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float player1DirectionAngle = player1.getDirectionAngle().x;
	float player2DirectionAngle = player2.getDirectionAngle().x;

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 model = glm::mat4(1.0f);
	// finally, get pikachu to its position
	model = glm::translate(model, player1.getPosition());
	// fourth, rotate pikachu to look at the right direction
	model = glm::translate(model, +player1.getSize() / 2.0f);
	model = glm::rotate(model, player1DirectionAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -player1.getSize() / 2.0f);
	// third, apply scaling to match model's size to bounding box (should be done after rotating model)
	model = glm::scale(model, player1.getSize() / glm::vec3(rotation * glm::vec4(pikachu.getSize(), 1.0f)));
	// second, rotate pikachu so that the head towards to the positive z-axis
	model = model * rotation;
	// first, align pikachu's left-bottom-back point to origin
	model = glm::translate(model, -pikachu.getMin());
	shader.setMat4("model", model);

	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	pikachu.Draw(shader);

	model = glm::mat4(1.0f);
	// finally, get pikachu to its position
	model = glm::translate(model, player2.getPosition());
	// fourth, rotate pikachu to look at the right direction
	model = glm::translate(model, +player2.getSize() / 2.0f);
	model = glm::rotate(model, glm::radians(180.0f) - player2DirectionAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -player2.getSize() / 2.0f);
	// third, apply scaling to match model's size to bounding box (should be done after rotating model)
	model = glm::scale(model, player2.getSize() / glm::vec3(rotation * glm::vec4(pikachu.getSize(), 1.0f)));
	// second, rotate pikachu so that the head towards to the positive z-axis
	model = model * rotation;
	// first, align pikachu's left-bottom-back point to origin
	model = glm::translate(model, -pikachu.getMin());
	shader.setMat4("model", model);

	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	pikachu.Draw(shader);

	glm::vec3 normalized_velocity = ball.getVelocity() / sqrt(glm::dot(ball.getVelocity(), ball.getVelocity()));
	float theta = acos(glm::dot(normalized_velocity, glm::vec3(1.0f, 0.0f, 0.0f)));
	theta = ball.getVelocity().y > 0 ? theta : -theta;

	model = glm::mat4(1.0f);
	model = glm::translate(model, ball.getPosition());
	model = glm::translate(model, +glm::vec3(ball.getRadius()));
	model = glm::rotate(model, theta, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -glm::vec3(ball.getRadius()));
	model = glm::scale(model, glm::vec3(ball.getRadius() * 2) / pokeball.getSize());
	model = model * rotation;
	model = glm::translate(model, -pokeball.getMin());
	shader.setMat4("model", model);

	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	pokeball.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::scale(model, MAP_SIZE / map.getSize());
	model = glm::translate(model, -map.getMin());
	shader.setMat4("model", model);

	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	map.Draw(shader);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	textRenderer.RenderText("10", 50.0f, 50.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}