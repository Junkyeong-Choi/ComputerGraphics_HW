#pragma once

#include "object.h"

class RectangleObject : public Object {
private:
	glm::vec2 size;
public:
	RectangleObject() {}
	~RectangleObject() {}
	RectangleObject(glm::vec2 _position, glm::vec2 _size) : Object(_position), size(_size) {}
	glm::vec2 getSize() { return size; }
	void setsize(glm::vec2 _size) { size = _size; }
};