#include "RenderItem.h"
#include "Object/Model.h"

RenderItem::~RenderItem() {
	for (auto& skinCluster : animation_.skinClusters) {
		ClearSkinCluster(skinCluster.second);
	}
}

/// <summary>
/// レンダリング情報の初期化
/// </summary>
void RenderItem::Initialize() {
	materialInfo_.Initialize();
	worldTransform_.Initialize(false);
	animation_.Initialize();
}

void RenderItem::Update() {
	materialInfo_.UpdateMatrix();
	worldTransform_.UpdateWorld();
	animation_.Update();
}

void RenderItem::UpdateGltf(Mesh mesh, int index) {
	meshWorldTransforms_[index].data_ = worldTransform_.data_;
	meshWorldTransforms_[index].worldMatrix_ = worldTransform_.worldMatrix_;

	auto it = animation_.rootNode.nodeMap.find(mesh.name);
	if (it != animation_.rootNode.nodeMap.end()) {
		meshWorldTransforms_[index].NodeUpdate(animation_.rootNode.nodes[animation_.rootNode.nodeMap[mesh.name]].localMatrix);
	}
	else {
		meshWorldTransforms_[index].UpdateWorld();
	}
}

void RenderItem::SetModel(Model* model) {

	for (int index = 0; index < model->GetMeshs().size(); index++) {
		WorldTransform transform;
		transform.Initialize(false);
		meshWorldTransforms_.push_back(std::move(transform));
	}

	animation_.SetModel(model);
}

void RenderItem::SetAnimation(std::list<AnimationData> datas) {
	animation_.SetAnimation(datas);
}