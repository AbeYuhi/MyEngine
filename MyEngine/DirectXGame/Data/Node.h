#pragma once
#include <string>
#include <vector>
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
	std::vector<Node> children;
	bool isMove;
};

Node ReadNode(aiNode* node);

Node InitializeNode(Node node);

Node InitializeNode(Node node, Node InitNode);

Node UpdateNode(Node node, NodeAnimation info, float time);

Matrix4x4* FindMatix(Node node, std::string meshName);