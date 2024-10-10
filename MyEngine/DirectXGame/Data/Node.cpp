#include "Node.h"

int32_t ReadNode(aiNode* node, std::vector<Node>& nodes) {
	Node result;
	result.name = node->mName.C_Str();
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);
	result.transform.scale_ = { scale.x, scale.y, scale.z };
	result.transform.rotate_ = Normalize({ rotate.x, -rotate.y , -rotate.z , rotate.w });
	result.transform.translate_ = { -translate.x, translate.y, translate.z };
	result.localMatrix = MakeAffineMatrix(result.transform.scale_, result.transform.rotate_, result.transform.translate_);
	result.index = int32_t(nodes.size());
	result.isMove = false;
	nodes.push_back(result);

	nodes[result.index].children.resize(node->mNumChildren);
	for (uint32_t index = 0; index < node->mNumChildren; index++) {
		//再帰的に読む
		nodes[result.index].children[index] = ReadNode(node->mChildren[index], nodes);
	}

	//自身のindexを返す
	return result.index;
}

RootNode CreateRootNode(aiNode* aiNode) {
	RootNode rootNode;
	rootNode.rootNode = ReadNode(aiNode, rootNode.nodes);

	//名前とindexのマッピングを行いアクセスしやすくする
	for (const Node& node : rootNode.nodes) {
		rootNode.nodeMap.emplace(node.name, node.index);
	}

	return rootNode;
}

Node UpdateNode(Node node, NodeAnimation info, float time) {

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

	return node;
}