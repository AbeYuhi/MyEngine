#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "Math/Matrix4x4.h"
#include "Math/Math.h"
#include "Data/Transform.h"
#include "Data/AnimationData.h"

struct Node {
	QuaternionTransformData transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	bool isMove;
};

struct RootNode {
	int32_t rootNode;
	std::vector<Node> nodes;
	std::map<std::string, uint32_t> nodeMap;
};

int32_t ReadNode(aiNode* node, std::vector<Node>& nodes);

RootNode CreateRootNode(aiNode* aiNode);

Node UpdateNode(Node node, NodeAnimation info, float time);