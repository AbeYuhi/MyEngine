#pragma once
#include "MaterialInfo.h"
#include "Transform.h"

struct RenderItem {
	WorldTransform worldTransform_;
	MaterialInfo materialInfo_;

	void Initialize(bool isSprite) {
		materialInfo_.Initialize();
		worldTransform_.Initialize(isSprite);
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
	}
};