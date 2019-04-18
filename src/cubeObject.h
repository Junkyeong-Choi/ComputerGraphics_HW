#pragma once

#include "object.h"

class CubeObject : public Object {
protected:
	glm::vec3 size;
public:
	CubeObject() {}
	~CubeObject() {}
	CubeObject(glm::vec3 _position, glm::vec3 _size) : Object(_position), size(_size) {}
	glm::vec3 getSize() { return size; }
	void setsize(glm::vec3 _size) { size = _size; }
};