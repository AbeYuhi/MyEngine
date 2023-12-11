#pragma once
#include <cstdint>
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"

struct Material {
	Vector4 color;
	int32_t enableLightint;
	int32_t isSpecularReflection;
	float padding[2];
	Matrix4x4 uvTransform;
	float shininess;
	Vector3 shininessColor;

	void Initialize();
};

struct ParticleMaterial {
	Vector4 color;
	Matrix4x4 uvTransform;

	void Initialize();
};