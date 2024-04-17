#include "Animation.h"

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
			NodeUpdate(*it);
		}

		it->preIsAnimation = it->isAnimation;
	}
	if (!isAnimation) {
		rootNode = initialNode;
	}

	//変更がなかったNodeを初期ポジに戻す
	rootNode = InitializeNode(rootNode, initialNode);
}

void Animation::NodeUpdate(AnimationInfo& info) {

	for (uint32_t channelIndex = 0; channelIndex < info.data.nodeAnimations.size(); channelIndex++) {

		rootNode = UpdateNode(rootNode, info.data.nodeAnimations[channelIndex], info.animationTime);
		
	}

	info.animationTime += 1.0f / 60.0f;
	if (info.animationTime > info.data.duration) {
		info.animationTime = 0.0f;
		info.isAnimation = false;
	}
}

void Animation::SetAnimation(std::list<AnimationData> datas) {

	for (auto it = datas.begin(); it != datas.end(); it++) {
		AnimationData data = *it;
		
		AnimationInfo info;
		info.data = data;
		info.isAnimation = false;
		info.preIsAnimation = false;
		info.animationTime = 0;
		infos.push_back(info);
	}

}