#include "Node.h"

Node ReadNode(aiNode* node) {
	Node result;
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);
	result.transform.scale_ = { scale.x, scale.y, scale.z };
	result.transform.rotate_ = Normalize({rotate.x, -rotate.y , -rotate.z , rotate.w});
	result.transform.translate_ = { -translate.x, translate.y, translate.z };
	result.localMatrix = MakeAffineMatrix(result.transform.scale_, result.transform.rotate_, result.transform.translate_);

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

		//Nodeがこのフレームで動いたか判断
		node.isMove = true;

		//位置
		Vector3 pos = CalculateValue(info.position, time);

		//回転
		Quaternion rotate = CalculateValue(info.rotation, time);

		//サイズ
		Vector3 scale = CalculateValue(info.scale, time);

		Matrix4x4 affineMatrix = MakeAffineMatrix(scale, Normalize(rotate), pos);
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