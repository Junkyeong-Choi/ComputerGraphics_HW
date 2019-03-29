#pragma once
#include <glm/mat4x4.hpp>

class SceneGraphNode {
private:
	glm::mat4 transform;
	void (*drawModel)(void);
	SceneGraphNode *child;
	SceneGraphNode *sibling;
public:
	SceneGraphNode(glm::mat4 _transform, void (*_drawModel)(void), SceneGraphNode *_child, SceneGraphNode *_sibling);
	~SceneGraphNode();
	void traverse(void);
};