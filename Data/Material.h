#pragma once
#include <cstdint>
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

struct Material {
	Vector4 color;
	int32_t enableLightint;
	float padding[3];
	Matrix4x4 uvTransform;

	void Initialize();
};