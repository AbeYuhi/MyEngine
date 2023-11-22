#pragma once
#include "MaterialInfo.h"
#include "Transform.h"

struct RenderItem {
	WorldTransform worldTransform_;
	MaterialInfo materialInfo_;

	void Initialize(const Matrix4x4* viewProjectionMatrix) {
		materialInfo_.Initialize();
		worldTransform_.Initialize(viewProjectionMatrix);
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
	}
};