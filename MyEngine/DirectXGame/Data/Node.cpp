#include "Node.h"

Node ReadNode(aiNode* node) {
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;
	aiLocalMatrix.Transpose();
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			result.localMatrix.m[y][x] = aiLocalMatrix[y][x];
		}
	}

	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		//再帰的に読む
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

Node InitializeNode(Node node) {

	node.localMatrix = MakeIdentity4x4();
	for (uint32_t childIndex = 0; childIndex < node.children.size(); childIndex++) {
		//再帰的に読む
		node.children[childIndex] = InitializeNode(node.children[childIndex]);
	}

	return node;
}

Node UpdateNode(Node node, AnimationChannel info, uint32_t frame) {

	if (node.name == info.name) {

		//位置
		Vector3 pos;
		if (info.numPositionChannel == 1) {
			pos = info.positionChannel[0].position;
		}
		else if (frame < info.numPositionChannel) {
			pos = info.positionChannel[frame].position;
		}
		else {
			pos = info.positionChannel[info.numPositionChannel - 1].position;
		}

		//回転
		Vector3 rotate;
		if (info.numRotateChannel == 1) {
			rotate = info.rotationChannel[0].rotation;
		}
		else if (frame < info.numRotateChannel) {
			rotate = info.rotationChannel[frame].rotation;
		}
		else {
			rotate = info.rotationChannel[info.numRotateChannel - 1].rotation;
		}

		//サイズ
		Vector3 scale;
		if (info.numScaleChannel == 1) {
			scale = info.scaleChannel[0].scale;
		}
		else if (frame < info.numScaleChannel) {
			scale = info.scaleChannel[frame].scale;
		}
		else {
			scale = info.scaleChannel[info.numScaleChannel - 1].scale;
		}

		Matrix4x4 affineMatrix = MakeAffineMatrix(scale, rotate, pos);
		node.localMatrix = Multiply(node.localMatrix, affineMatrix);
	}
	else {
		for (uint32_t childIndex = 0; childIndex < node.children.size(); childIndex++) {
			//再帰的に読む
			node.children[childIndex] = UpdateNode(node.children[childIndex], info, frame);
		}
	}

	return node;
}