#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "model.h"

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

	void init() {
		shader = Shader("./src/shader.vert", "./src/shader.frag");
		pikachu = Model("./resources/models/pikachu/Pikachu.obj");
		pokeball = Model("./resources/models/pokeball/Pokeball.obj");
		map = Model("./resources/models/box/box.obj");
	}

	void setScreenSize(int _width, int _height) {
		width = _width;
		height = _height;
	}

	void render() {
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		pikachu.Draw(shader);
		pokeball.Draw(shader);
		map.Draw(shader);
	}
};