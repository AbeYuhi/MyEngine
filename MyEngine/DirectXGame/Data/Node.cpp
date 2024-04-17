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

	node.isMove = false;
	node.localMatrix = MakeIdentity4x4();
	for (uint32_t childIndex = 0; childIndex < node.children.size(); childIndex++) {
		//再帰的に読む
		node.children[childIndex] = InitializeNode(node.children[childIndex]);
	}

	return node;
}

Node InitializeNode(Node node, Node InitNode) {

	if (!node.isMove) {
		node.localMatrix = InitNode.localMatrix;
	}

	for (uint32_t childIndex = 0; childIndex < node.children.size(); childIndex++) {
		//再帰的に読む
		node.children[childIndex] = InitializeNode(node.children[childIndex], InitNode.children[childIndex]);
	}

	return node;
}

Node UpdateNode(Node node, NodeAnimation info, float time) {

	if (node.name == info.nodeName) {

		node.isMove = true;

		//位置
		Vector3 pos = CalculateValue(info.position, time);

		//回転
		Quaternion rotate = CalculateValue(info.rotation, time);

		//サイズ
		Vector3 scale = CalculateValue(info.scale, time);

		Matrix4x4 affineMatrix = MakeAffineMatrix(scale, rotate, pos);
		node.localMatrix = Multiply(node.localMatrix, affineMatrix);
	}
	else {
		for (uint32_t childIndex = 0; childIndex < node.children.size(); childIndex++) {
			//再帰的に読む
			node.children[childIndex] = UpdateNode(node.children[childIndex], info, time);
		}
	}

	return node;
}

Matrix4x4* FindMatix(Node node, std::string meshName) {

	if (node.name == meshName) {
		return &node.localMatrix;
	}
	else {
		for (uint32_t childIndex = 0; childIndex < node.children.size(); childIndex++) {
			//再帰的に読む
			if (FindMatix(node.children[childIndex], meshName)) {
				return FindMatix(node.children[childIndex], meshName);
			}
		}
	}

	return nullptr;
}