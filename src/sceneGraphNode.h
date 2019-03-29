#pragma once
#include <glm/mat4x4.hpp>

class SceneGraphNode {
private:
	glm::mat4x4 transform;
	void (*drawModel)(void);
	SceneGraphNode *sibling;
	SceneGraphNode *child;
public:
	SceneGraphNode(glm::mat4x4 _transform, void (*_drawModel)(void), SceneGraphNode *_sibling, SceneGraphNode *_child);
	~SceneGraphNode();
	void traverse(void);
};