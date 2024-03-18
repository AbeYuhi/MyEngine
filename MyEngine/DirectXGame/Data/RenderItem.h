#pragma once
#include <vector>
#include "MaterialInfo.h"
#include "Transform.h"

struct RenderItem {
	WorldTransform worldTransform_;
	std::vector<WorldTransform> meshWorldTransforms_;
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
		meshWorldTransforms_.clear();
	}

	void UpdateGltf(Matrix4x4 localMatrix) {
		WorldTransform transform;
		transform.Initialize(false);
		transform.worldMatrix_ = worldTransform_.worldMatrix_;
		transform.NodeUpdate(localMatrix);
		meshWorldTransforms_.push_back(transform);
	}
};