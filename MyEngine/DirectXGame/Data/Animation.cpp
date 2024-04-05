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
			it->frame = 0;
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

}

void Animation::NodeUpdate(AnimationInfo& info) {

	for (uint32_t channelIndex = 0; channelIndex < info.data.numChannels; channelIndex++) {

		rootNode = UpdateNode(rootNode, info.data.channels[channelIndex], info.frame);
		
	}

	if (info.frame >= info.data.numFrames) {
		info.frame = 0;
	}
	else {
		info.frame++;
	}
}

void Animation::SetAnimation(std::list<AnimationData> datas) {

	for (auto it = datas.begin(); it != datas.end(); it++) {
		AnimationData data = *it;
		
		AnimationInfo info;
		info.data = data;
		info.isAnimation = false;
		info.preIsAnimation = false;
		info.frame = 0;
		infos.push_back(info);
	}

}