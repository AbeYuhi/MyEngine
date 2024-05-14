#pragma once
#include <vector>
#include <span>
#include <WRL/client.h>
#include "External/DirectXTex/d3dx12.h"
#include "Math/Matrix4x4.h"

using namespace Microsoft::WRL;

struct SkinCluster {
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence>
};
