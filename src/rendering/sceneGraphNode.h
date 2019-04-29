#pragma once
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "model.h"
#include "shader.h"

class SceneGraphNode {
private:
	glm::mat4 transform;
	Model* model;
	glm::vec3 color;
	SceneGraphNode *child;
	SceneGraphNode *sibling;
public:
	SceneGraphNode(glm::mat4 _transform, Model* _model, glm::vec3 _color, SceneGraphNode * _child, SceneGraphNode * _sibling) :
		transform(_transform), model(_model), color(_color), sibling(_sibling), child(_child) {}
	~SceneGraphNode() {
		delete child;
		delete sibling;
	}
	void traverse(Shader shader, glm::mat4 acc) {
		shader.setVec3("aColor", color);
		shader.setMat4("model", acc * transform);
		if (model != NULL)
			model->Draw(shader);
		if (child != NULL)
			child->traverse(shader, acc * transform);
		if (sibling != NULL)
			sibling->traverse(shader, acc);
	}
};