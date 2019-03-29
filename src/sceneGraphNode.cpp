#include "sceneGraphNode.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

SceneGraphNode::SceneGraphNode(glm::mat4x4 _transform, void(*_drawModel)(void), SceneGraphNode * _sibling, SceneGraphNode * _child):
	transform(_transform), drawModel(_drawModel), sibling(_sibling), child(_child)
{
}

SceneGraphNode::~SceneGraphNode() {
	delete child;
	delete sibling;
}

void SceneGraphNode::traverse(void) {
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(transform));
	drawModel();
	if (child != NULL)
		child->traverse();
	glPopMatrix();
	if (sibling != NULL)
		sibling->traverse();
}