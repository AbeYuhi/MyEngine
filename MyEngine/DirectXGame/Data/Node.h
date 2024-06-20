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

//struct Node {
//	QuaternionTransformData transform;
//	Matrix4x4 localMatrix;
//	std::string name;
//	std::vector<Node> children;
//	bool isMove;
//};

struct Node {
	QuaternionTransformData transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
	bool isMove;
};

struct RootNode {
	int32_t rootNode;
	std::vector<Node> nodes;
	std::map<std::string, uint32_t> nodeMap;
};

Node ReadNode(aiNode* node);

int32_t ReadNode(aiNode* node, const std::optional<int32_t>& parent, std::vector<Node>& nodes);

RootNode CreateRootNode(aiNode* node);

Node InitializeNode(Node node);

Node InitializeNode(Node node, Node InitNode);

Node UpdateNode(Node node, NodeAnimation info, float time);

Matrix4x4* FindMatix(Node node, std::string meshName);