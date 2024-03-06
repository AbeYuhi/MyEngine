#pragma once
#include "MaterialInfo.h"
#include "Transform.h"

struct RenderItem {
	WorldTransform worldTransform_;
	MaterialInfo materialInfo_;

	/// <summary>
	/// レンダリング情報の初期化
	/// </summary>
	void Initialize() {
		materialInfo_.Initialize();
		worldTransform_.Initialize(false);
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
	}

	void UpdateGltf(Matrix4x4 localMatrix) {
		worldTransform_.NodeUpdate(localMatrix);
	}
};