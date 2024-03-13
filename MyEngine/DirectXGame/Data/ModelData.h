#pragma once
#include <vector>
#include "VertexData.h"
#include "IndexData.h"
#include "Node.h"
#include "Material.h"
#include "MaterialData.h"

struct ModelData {
	std::vector<VertexData> vertices;
	std::vector<IndexData> indices;
	MaterialData material;
};