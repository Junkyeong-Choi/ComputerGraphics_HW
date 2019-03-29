#include "electricityObject.h"

ElectricityObject::ElectricityObject() {
	foldingPointsInfo.push_back(PointInfo{ glm::vec2(-3,1),-0.05,1 });
	foldingPointsInfo.push_back(PointInfo{ glm::vec2(-2,-1),0.05,2 });
	foldingPointsInfo.push_back(PointInfo{ glm::vec2(-1,1),-0.05,1 });
	foldingPointsInfo.push_back(PointInfo{ glm::vec2(0,-1),0.05,3 });
	foldingPointsInfo.push_back(PointInfo{ glm::vec2(1,1),-0.05,1 });
	foldingPointsInfo.push_back(PointInfo{ glm::vec2(2,-1),0.05,2 });
	foldingPointsInfo.push_back(PointInfo{ glm::vec2(3,1),-0.05,1 });
}

ElectricityObject::~ElectricityObject() {
	// remove vector memory
	std::vector<PointInfo>().swap(foldingPointsInfo);
}

void ElectricityObject::move(int delta) {
	std::vector<PointInfo>::iterator iter;
	for (iter = foldingPointsInfo.begin(); iter != foldingPointsInfo.end(); iter++) {
		iter->pointPosition.y += iter->pointVelocity * delta;
		if (iter->pointPosition.y > iter->vibrationLimit || iter->pointPosition.y < -iter->vibrationLimit) {
			iter->pointVelocity = -iter->pointVelocity;
			if (iter->pointPosition.y > iter->vibrationLimit)
				iter->pointPosition.y = iter->vibrationLimit;
			else
				iter->pointPosition.y = -iter->vibrationLimit;
		}
			
	}
}