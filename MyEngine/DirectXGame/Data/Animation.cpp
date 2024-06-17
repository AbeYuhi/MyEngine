#include "Animation.h"
#include "Object/Model.h"

void Animation::Initialize() {

}

void Animation::Update() {

	//rootNodeの初期化
	rootNode = InitializeNode(rootNode);

	//アニメーションの更新
	bool isAnimation = false;
	for (auto it = infos.begin(); it != infos.end(); it++) {

		if (it->isAnimation != it->preIsAnimation) {
			it->animationTime = 0;
		}

		if (it->isAnimation && it->data.name != "None") {
			isAnimation = true;
			//アニメーションの時間を進める
			if (it->animationSpeed < 0.0f) {
				it->animationTime += 1.0f / 60.0f * it->animationSpeed;
				if (it->animationTime < 0.0f) {
					it->animationTime = it->data.duration;
					if (!it->isLoop) {
						it->isAnimation = false;
					}
				}
			}
			else {
				it->animationTime += 1.0f / 60.0f * it->animationSpeed;
				if (it->animationTime > it->data.duration) {
					it->animationTime = 0;
					if (!it->isLoop) {
						it->isAnimation = false;
					}
				}
			}

			//Nodeのアップデートをする
			NodeUpdate(*it);

			//Jointの更新
			ApplyAnimation(*it);

			//Skeltonの更新
			SkeletonUpdate();

			//SkinClusterの更新
			SkinClusterUpdate();
		}

		it->preIsAnimation = it->isAnimation;
	}
	if (!isAnimation) {
		rootNode = initialNode;
		skeleton = CreateSkeleton(initialNode);
		SkinClusterUpdate();
	}

	//変更がなかったNodeを初期ポジに戻す
	rootNode = InitializeNode(rootNode, initialNode);
}

void Animation::NodeUpdate(AnimationInfo& info) {

	for (uint32_t channelIndex = 0; channelIndex < info.data.nodeAnimations.size(); channelIndex++) {

		if (info.data.nodeAnimations[channelIndex].isMeshNode) {
			rootNode = UpdateNode(rootNode, info.data.nodeAnimations[channelIndex], info.animationTime);
		}
	}
}

void Animation::SkeletonUpdate() {
	//すべてのJointを更新。親が若いので通常ループで処理が可能
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale_, Normalize(joint.transform.rotate_), joint.transform.translate_);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void Animation::ApplyAnimation(AnimationInfo& info) {
	for (Joint& joint : skeleton.joints) {
		for (int index = 0; index < info.data.nodeAnimations.size();index++) {
			if (joint.name == info.data.nodeAnimations[index].nodeName) {
				const NodeAnimation& rootNodeAnimation = info.data.nodeAnimations[index];
				joint.transform.translate_ = CalculateValue(rootNodeAnimation.position, info.animationTime);
				joint.transform.rotate_ = CalculateValue(rootNodeAnimation.rotation, info.animationTime);
				joint.transform.scale_ = CalculateValue(rootNodeAnimation.scale, info.animationTime);
			}
		}
	}
}

void Animation::SkinClusterUpdate() {
	for (auto& skinCluster : skinClusters) {
		for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); jointIndex++) {
			assert(jointIndex < skinCluster.second.inverseBindPoseMatrices.size());
			skinCluster.second.mappedPalette[jointIndex].skeletonSpaceMatrix =
				skinCluster.second.inverseBindPoseMatrices[jointIndex] * skeleton.joints[jointIndex].skeletonSpaceMatrix;
			skinCluster.second.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix =
				Transpose(Inverse(skinCluster.second.mappedPalette[jointIndex].skeletonSpaceMatrix));
		}
	}
}

void Animation::SkeletonDraw() {

}

void Animation::SetAnimation(std::list<AnimationData> datas) {
	for (auto it = datas.begin(); it != datas.end(); it++) {
		AnimationData data = *it;
		
		AnimationInfo info;
		info.data = data;
		info.isAnimation = false;
		info.preIsAnimation = false;
		info.isLoop = false;
		info.animationTime = 0;
		info.animationSpeed = 1.0f;
		infos.push_back(info);
	}
}

void Animation::SetModel(Model* model) {
	initialNode = model->GetInialNode();
	skeleton = CreateSkeleton(initialNode);
	for (auto& mesh : model->GetMeshs()) {
		skinClusters[mesh.name] = CreateSkinCluster(skeleton, mesh.modelData);
	}
	SkinClusterUpdate();
}

void Animation::SetAnimationSpeed(std::string animationName, float speed) {
	for (auto it = infos.begin(); it != infos.end(); it++) {
		if (it->data.name == animationName) {
			it->animationSpeed = speed;
		}
	}
}
