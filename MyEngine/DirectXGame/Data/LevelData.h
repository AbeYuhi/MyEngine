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
		std::string tag;
		Vector3 centerPos;
		Vector3 rotate;
		Vector3 size;
		float radius;
		bool collisionCheck;
	};
	struct ObjectData {
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
		
		std::string objName;
		std::string fileName;
		bool drawCheck;
		std::optional<ObjectCollider> collider;
		std::optional<int32_t> parent;
	};

	std::vector<ObjectData> objects;
};