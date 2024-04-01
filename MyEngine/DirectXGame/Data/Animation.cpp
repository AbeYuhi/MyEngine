#include "Animation.h"

void Animation::Initialize() {

}

void Animation::Update() {

	NodeUpdate();

}

void Animation::NodeUpdate() {

	//アニメーションの更新
	if (isAnimation) {

		if (data.name == "None") {

			rootNode = initialNode;

			return;
		}

		for (uint32_t channelIndex = 0; channelIndex < data.numChannels; channelIndex++) {

			if (rootNode.name == data.channels[channelIndex].name) {
				//位置
				Vector3 pos;
				if (data.channels[channelIndex].numPositionChannel == 1) {
					pos = data.channels[channelIndex].positionChannel[0].position;
				}
				else if (frame < data.channels[channelIndex].numPositionChannel) {
					pos = data.channels[channelIndex].positionChannel[frame].position;
				}
				else {
					pos = data.channels[channelIndex].positionChannel[data.channels[channelIndex].numPositionChannel - 1].position;
				}

				//回転
				Vector3 rotate;
				if (data.channels[channelIndex].numRotateChannel == 1) {
					rotate = data.channels[channelIndex].rotationChannel[0].rotation;
				}
				else if (frame < data.channels[channelIndex].numRotateChannel) {
					rotate = data.channels[channelIndex].rotationChannel[frame].rotation;
				}
				else {
					rotate = data.channels[channelIndex].rotationChannel[data.channels[channelIndex].numRotateChannel - 1].rotation;
				}

				//サイズ
				Vector3 scale;
				if (data.channels[channelIndex].numScaleChannel == 1) {
					scale = data.channels[channelIndex].scaleChannel[0].scale;
				}
				else if (frame < data.channels[channelIndex].numScaleChannel) {
					scale = data.channels[channelIndex].scaleChannel[frame].scale;
				}
				else {
					scale = data.channels[channelIndex].scaleChannel[data.channels[channelIndex].numScaleChannel - 1].scale;
				}

				Matrix4x4 affineMatrix = MakeAffineMatrix(scale, rotate, pos);
				rootNode.localMatrix = affineMatrix;
			}
			else {
				for (uint32_t nodeIndex = 0; nodeIndex < rootNode.children.size(); nodeIndex++) {
					if (rootNode.children[nodeIndex].name == data.channels[channelIndex].name) {
						//位置
						Vector3 pos;
						if (data.channels[channelIndex].numPositionChannel == 1) {
							pos = data.channels[channelIndex].positionChannel[0].position;
						}
						else if (frame < data.channels[channelIndex].numPositionChannel) {
							pos = data.channels[channelIndex].positionChannel[frame].position;
						}
						else {
							pos = data.channels[channelIndex].positionChannel[data.channels[channelIndex].numPositionChannel - 1].position;
						}

						//回転
						Vector3 rotate;
						if (data.channels[channelIndex].numRotateChannel == 1) {
							rotate = data.channels[channelIndex].rotationChannel[0].rotation;
						}
						else if (frame < data.channels[channelIndex].numRotateChannel) {
							rotate = data.channels[channelIndex].rotationChannel[frame].rotation;
						}
						else {
							rotate = data.channels[channelIndex].rotationChannel[data.channels[channelIndex].numRotateChannel - 1].rotation;
						}

						//サイズ
						Vector3 scale;
						if (data.channels[channelIndex].numScaleChannel == 1) {
							scale = data.channels[channelIndex].scaleChannel[0].scale;
						}
						else if (frame < data.channels[channelIndex].numScaleChannel) {
							scale = data.channels[channelIndex].scaleChannel[frame].scale;
						}
						else {
							scale = data.channels[channelIndex].scaleChannel[data.channels[channelIndex].numScaleChannel - 1].scale;
						}

						Matrix4x4 affineMatrix = MakeAffineMatrix(scale, rotate, pos);
						rootNode.children[nodeIndex].localMatrix = affineMatrix;
					}
				}
			}
		}

		if (frame >= data.numFrames) {
			frame = 0;
		}
	}
	else {
		rootNode = initialNode;
	}

}