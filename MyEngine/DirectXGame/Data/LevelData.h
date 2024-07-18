#pragma once
#include <string>
#include <memory>
#include <list>
#include <vector>
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
		
		std::string objName;
		std::string fileName;
		std::optional<ObjectCollider> collider;
		std::optional<int32_t> parent;
	};

	std::vector<ObjectData> objects;
};