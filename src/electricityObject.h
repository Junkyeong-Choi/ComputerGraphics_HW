#pragma once
#include <glm/vec2.hpp>
#include <vector>

typedef struct PointInfo {
	glm::vec2 pointPosition;
	float pointVelocity;
	float vibrationLimit;
}PointInfo;

class ElectricityObject {
private:
	std::vector<PointInfo> foldingPointsInfo;
public:
	ElectricityObject();
	~ElectricityObject();
	void move(int delta);
};