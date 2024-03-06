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