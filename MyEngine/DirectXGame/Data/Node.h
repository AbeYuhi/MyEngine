#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "Math/Matrix4x4.h"
#include "Math/Math.h"
#include "Data/AnimationData.h"

struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

Node ReadNode(aiNode* node);

Node InitializeNode(Node node);

Node UpdateNode(Node node, AnimationChannel info, uint32_t frame);

Matrix4x4 FindMatix(Node node, std::string meshName);