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

		for (int index = 0; index < 10; index++) {
			WorldTransform transform;
			transform.Initialize(false);
			meshWorldTransforms_.push_back(transform);
		}
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
		animationInfo_.Update();
	}

	void UpdateGltf(Mesh mesh, int index) {
		if (index >= 10) {
			assert(false);
		}

		if (mesh.name == animationInfo_.rootNode.name) {		
			meshWorldTransforms_[index].worldMatrix_ = worldTransform_.worldMatrix_;
			meshWorldTransforms_[index].NodeUpdate(animationInfo_.rootNode.localMatrix);
		}
		else {
			for (uint32_t nodeIndex = 0; nodeIndex < animationInfo_.rootNode.children.size(); nodeIndex++) {
				if (mesh.name == animationInfo_.rootNode.children[nodeIndex].name) {
					meshWorldTransforms_[index].worldMatrix_ = worldTransform_.worldMatrix_;
					meshWorldTransforms_[index].NodeUpdate(animationInfo_.rootNode.children[nodeIndex].localMatrix);
					break;
				}
			}
		}
	}
};