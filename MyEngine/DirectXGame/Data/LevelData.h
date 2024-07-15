#pragma once
#include <string>
#include "Math/Vector3.h"
#include "Data/Transform.h"

struct LevelData {
	struct ObjectCollider {
		std::string type;
		Vector3 centerPos;
		Vector3 size;
	};
	struct ObjectData {
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
		
		std::string fileName;
		ObjectCollider collider;
		ObjectData* parent;
	};

	std::vector<ObjectData> objects;
};