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
	bool hiddenLineRemoval;
public:
	SceneGraphNode(glm::mat4 _transform, Model* _model, glm::vec3 _color, bool _hiddenLineRemoval, SceneGraphNode * _child, SceneGraphNode * _sibling) :
		transform(_transform), model(_model), color(_color), sibling(_sibling), child(_child), hiddenLineRemoval(_hiddenLineRemoval) {}
	~SceneGraphNode() {
		delete child;
		delete sibling;
	}
	void traverse(Shader shader, glm::mat4 acc) {
		shader.setVec3("aColor", color);
		shader.setMat4("model", acc * transform);
		if (model != NULL) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			shader.setBool("isBackgroundColor", false);
			model->Draw(shader);

			if (hiddenLineRemoval) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0f, 1.0f);
				shader.setBool("isBackgroundColor", true);
				model->Draw(shader);
				glDisable(GL_POLYGON_OFFSET_FILL);
			}
		}
		if (child != NULL)
			child->traverse(shader, acc * transform);
		if (sibling != NULL)
			sibling->traverse(shader, acc);
	}
};