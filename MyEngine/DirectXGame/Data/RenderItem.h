#pragma once
#include <vector>
#include "Animation.h"
#include "MaterialInfo.h"
#include "Transform.h"
#include "Mesh.h"

struct RenderItem {
	WorldTransform worldTransform_;
	std::vector<WorldTransform> meshWorldTransforms_;
	MaterialInfo materialInfo_;
	Animation animation_;

	~RenderItem() {
		for (auto& skinCluster : animation_.skinClusters) {
			if (SkinCluster::isAlive[skinCluster.index]) {
				ClearSkinCluster(skinCluster);
			}
		}
	}

	/// <summary>
	/// レンダリング情報の初期化
	/// </summary>
	void Initialize() {
		materialInfo_.Initialize();
		worldTransform_.Initialize(false); 
		animation_.Initialize();

		for (int index = 0; index < 10; index++) {
			WorldTransform transform;
			transform.Initialize(false);
			meshWorldTransforms_.push_back(transform);
		}

#ifdef _DEBUG
		for (int index = 0; index < 10; index++) {
			WorldTransform transform;
			transform.Initialize(false);
			meshWorldTransforms_.push_back(transform);
		}
#endif // _DEBUG
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
		animation_.Update();
	}

	void UpdateGltf(Mesh mesh, int index) {
		if (index >= 10) {
			assert(false);
		}

		meshWorldTransforms_[index].worldMatrix_ = worldTransform_.worldMatrix_;
		if (FindMatix(animation_.rootNode, mesh.name)) {
			meshWorldTransforms_[index].NodeUpdate(*FindMatix(animation_.rootNode, mesh.name));
		}
		else {
			meshWorldTransforms_[index].UpdateWorld();
		}
	}
};