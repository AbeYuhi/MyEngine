#pragma once
#include <vector>
#include "VertexData.h"
#include "Node.h"
#include "Material.h"
#include "MaterialData.h"

struct ModelData {
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
};