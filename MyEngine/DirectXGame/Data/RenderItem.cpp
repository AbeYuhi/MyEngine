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
	if (FindMatix(animation_.rootNode, mesh.name)) {
		meshWorldTransforms_[index].NodeUpdate(*FindMatix(animation_.rootNode, mesh.name));
	}
	else {
		meshWorldTransforms_[index].UpdateWorld();
	}
}

void RenderItem::SetModel(Model* model) {

	for (int index = 0; index < model->GetMeshs().size(); index++) {
		WorldTransform transform;
		transform.Initialize(false);
		meshWorldTransforms_.push_back(transform);
	}

	animation_.SetModel(model);
}

void RenderItem::SetAnimation(std::list<AnimationData> datas) {
	animation_.SetAnimation(datas);
}