#pragma once
#include <vector>
#include "Animation.h"
#include "MaterialInfo.h"
#include "Transform.h"
#include "Mesh.h"

class Model;

struct RenderItem {
	WorldTransform worldTransform_;
	std::vector<WorldTransform> meshWorldTransforms_;
	MaterialInfo materialInfo_;
	Animation animation_;

	~RenderItem();

	/// <summary>
	/// レンダリング情報の初期化
	/// </summary>
	void Initialize();

	void Update();

	void UpdateGltf(Mesh mesh, int index);

	void SetModel(Model* model);

	void SetAnimation(std::list<AnimationData> datas);
};