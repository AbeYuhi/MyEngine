#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include "Math/Matrix4x4.h"

struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

Node ReadNode(aiNode* node);