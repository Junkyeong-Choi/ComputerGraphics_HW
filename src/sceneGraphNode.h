#pragma once
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

class SceneGraphNode {
private:
	glm::mat4 transform;
	void (*drawModel)(void);
	SceneGraphNode *child;
	SceneGraphNode *sibling;
public:
	SceneGraphNode(glm::mat4 _transform, void(*_drawModel)(void), SceneGraphNode * _child, SceneGraphNode * _sibling) :
		transform(_transform), drawModel(_drawModel), sibling(_sibling), child(_child) {}
	~SceneGraphNode() {
		delete child;
		delete sibling;
	}
	void traverse(void) {
		glPushMatrix();
		glMultMatrixf(glm::value_ptr(transform));
		if (drawModel != NULL)
			drawModel();
		if (child != NULL)
			child->traverse();
		glPopMatrix();
		if (sibling != NULL)
			sibling->traverse();
	}
};