#pragma once
#include <string>
#include "Math/Vector3.h"
#include "Data/Transform.h"

struct LevelData {
	struct ObjectData {
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;

		std::string fileName;
		ObjectData* parent;
	};

	std::vector<ObjectData> objects;
};