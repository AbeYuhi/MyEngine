#pragma once
#include <vector>
#include "AnimationInfo.h"
#include "MaterialInfo.h"
#include "Transform.h"
#include "Mesh.h"

struct RenderItem {
	WorldTransform worldTransform_;
	std::vector<WorldTransform> meshWorldTransforms_;
	MaterialInfo materialInfo_;
	AnimationInfo animationInfo_;

	/// <summary>
	/// レンダリング情報の初期化
	/// </summary>
	void Initialize() {
		materialInfo_.Initialize();
		worldTransform_.Initialize(false); 
		animationInfo_.Initialize();
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
		animationInfo_.Update();
		meshWorldTransforms_.clear();
	}

	void UpdateGltf(Mesh mesh) {
		if (mesh.name == animationInfo_.rootNode.name) {
			WorldTransform transform;
			transform.Initialize(false);
			transform.worldMatrix_ = worldTransform_.worldMatrix_;
			transform.NodeUpdate(animationInfo_.rootNode.localMatrix);
			meshWorldTransforms_.push_back(transform);
		}
		else {
			for (uint32_t nodeIndex = 0; nodeIndex < animationInfo_.rootNode.children.size(); nodeIndex++) {
				if (mesh.name == animationInfo_.rootNode.children[nodeIndex].name) {

					WorldTransform transform;
					transform.Initialize(false);
					transform.worldMatrix_ = worldTransform_.worldMatrix_;
					transform.NodeUpdate(animationInfo_.rootNode.children[nodeIndex].localMatrix);
					meshWorldTransforms_.push_back(transform);
					break;
				}
			}
		}
	}
};