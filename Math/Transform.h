#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

struct TransformMatrix {
	Matrix4x4 WVP_;
	Matrix4x4 World_;
};

struct TransformData{
	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;
};

struct WorldTransform {
	TransformData data_;
	TransformMatrix* matrix_;

	void UpdateWorld();
	void UpdateWVP(Matrix4x4 viewProjectionMatrix);
};