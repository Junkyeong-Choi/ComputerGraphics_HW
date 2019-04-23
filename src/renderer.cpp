#include "renderer.h"
#include "settings.h"
#include "game.h"
#include "sceneGraphNode.h"

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
	glViewport(0, 0, _width, _height);
	textRenderer.setScreenSize(_width, _height);
}

glm::mat4 Renderer::getViewMatrix(MovableCubeObject& player1, ViewMode viewmode, CameraForViewThree& cameraForViewThree) {
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
		eye = glm::vec3(cameraForViewThree.getPosition().x, cameraForViewThree.getPosition().y, MAP_SIZE.z * 3.0f);
		center = glm::vec3(MAP_SIZE.x / 2.0f, MAP_SIZE.y / 2.0f, 0.0f);
		up = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	return glm::lookAt(eye, center, up);
}

glm::mat4 Renderer::makePikachuModelMatrix(MovableCubeObject& player, bool isPlayer1) {
	float playerDirectionAngle = player.getDirectionAngle().x;
	if (!isPlayer1)
		playerDirectionAngle = glm::radians(180.0f) - playerDirectionAngle;

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 model = glm::mat4(1.0f);
	// finally, get pikachu to its position
	model = glm::translate(model, player.getPosition());
	// fourth, rotate pikachu to look at the right direction
	model = glm::translate(model, +player.getSize() / 2.0f);
	model = glm::rotate(model, playerDirectionAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -player.getSize() / 2.0f);
	// third, apply scaling to match model's size to bounding box (should be done after rotating model)
	model = glm::scale(model, player.getSize() / glm::vec3(rotation * glm::vec4(pikachu.getSize(), 1.0f)));
	// second, rotate pikachu so that the head towards to the positive z-axis
	model = model * rotation;
	// first, align pikachu's left-bottom-back point to origin
	model = glm::translate(model, -pikachu.getMin());

	return model;
}

glm::mat4 Renderer::makePokeballModelMatrix(BallObject& ball) {
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 normalized_velocity = ball.getVelocity() / sqrt(glm::dot(ball.getVelocity(), ball.getVelocity()));
	float theta = acos(glm::dot(normalized_velocity, glm::vec3(1.0f, 0.0f, 0.0f)));
	theta = ball.getVelocity().y > 0 ? theta : -theta;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, ball.getPosition());
	model = glm::translate(model, +glm::vec3(ball.getRadius()));
	model = glm::rotate(model, theta, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, -glm::vec3(ball.getRadius()));
	model = glm::scale(model, glm::vec3(ball.getRadius() * 2) / pokeball.getSize());
	model = model * rotation;
	model = glm::translate(model, -pokeball.getMin());

	return model;
}

glm::mat4 Renderer::makeMapModelMatrix() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, MAP_SIZE / map.getSize());
	model = glm::translate(model, -map.getMin());

	return model;
}

void Renderer::renderScene(MovableCubeObject& player1, MovableCubeObject& player2, BallObject& ball, ViewMode viewmode, CameraForViewThree& cameraForViewThree) {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 view = getViewMatrix(player1, viewmode, cameraForViewThree);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	SceneGraphNode* sceneGraph =
		new SceneGraphNode(makeMapModelMatrix() , &map, glm::vec3(1.0f), false,
			nullptr,
			new SceneGraphNode(makePikachuModelMatrix(player1, true), &pikachu, glm::vec3(1.0f, 1.0f, 0.0f), true,
				nullptr,
				new SceneGraphNode(makePikachuModelMatrix(player2, false), &pikachu, glm::vec3(1.0f, 1.0f, 0.0f), true,
					nullptr,
					new SceneGraphNode(makePokeballModelMatrix(ball), &pokeball, glm::vec3(1.0f, 0.0f, 0.0f), true,
						nullptr,
						nullptr
					)
				)
			)
		);

	sceneGraph->traverse(shader, glm::mat4(1.0f));

	delete sceneGraph;
}

void Renderer::renderText(ViewMode viewmode, GameState gamestate, int score1, int score2, int delayTime) {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	textRenderer.RenderText(std::to_string(score1), 30.0f, height - 10.0f - 48.0f, 1.0f);
	GLfloat text_width = textRenderer.TextWidth(std::to_string(score1), 1.0f);
	textRenderer.RenderText(std::to_string(score2), width - 30.0f - text_width, height - 10.0f - 48.0f, 1.0f);

	if (gamestate == GAME_READY) {
		text_width = textRenderer.TextWidth(std::to_string(delayTime / 1000 + 1), 1.0f);
		textRenderer.RenderText(std::to_string(delayTime / 1000 + 1), width / 2.0f - text_width / 2.0f, height - 10.0f - 48.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (gamestate == GAME_PLAYING && delayTime > 0) {
		text_width = textRenderer.TextWidth("GO!", 1.0f);
		textRenderer.RenderText("GO!", width / 2.0f - text_width / 2.0f, height - 10.0f - 48.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (gamestate == GAME_SCORE) {
		text_width = textRenderer.TextWidth("SCORE!", 1.0f);
		textRenderer.RenderText("SCORE!", width / 2.0f - text_width / 2.0f, height - 10.0f - 48.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (gamestate == GAME_SET) {
		string s;

		if (score1 > score2)
			s = "YOU WIN!";
		else
			s = "YOU LOSE ;(";

		text_width = textRenderer.TextWidth(s, 1.0f);
		textRenderer.RenderText(s, width / 2.0f - text_width / 2.0f, height - 10.0f - 48.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		text_width = textRenderer.TextWidth("(Press Enter)", 0.8f);
		textRenderer.RenderText("(Press Enter)", width / 2.0f - text_width / 2.0f, height - 10.0f - 96.0f, 0.8f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else {
		string s = "???";

		if (viewmode == VIEW_CHARACTER_EYE)
			s = "Eye View";
		else if (viewmode == VIEW_CHARACTER_BACK)
			s = "Back View";
		else if (viewmode == VIEW_CELLING)
			s = "Ceiling View";

		text_width = textRenderer.TextWidth(s + string(" (Spacebar to change)"), 0.5f);
		textRenderer.RenderText(s + string(" (Spacebar to change)"), width / 2.0f - text_width / 2.0f, height - 10.0f - 48.0f, 0.5f, glm::vec3(0.9f));
	}
}