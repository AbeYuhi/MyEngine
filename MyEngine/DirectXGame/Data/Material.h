#pragma once
#include <cstdint>
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"

struct Material {
	Vector4 color;
	int32_t enableLightint;
	int32_t isSpecularReflection;
	float padding0[2];
	Matrix4x4 uvTransform;
	float shininess;
	Vector3 shininessColor;
	int32_t isEnvironment;
	float padding1[3];
	float environmentCoefficient;

	void Initialize();
};

struct ParticleMaterial {
	Vector4 color;
	Matrix4x4 uvTransform;

	void Initialize();
};