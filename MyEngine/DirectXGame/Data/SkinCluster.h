#pragma once
#include <vector>
#include <array>
#include <span>
#include <map>
#include <algorithm>
#include <WRL/client.h>
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Math/Matrix4x4.h"
#include "Math/Math.h"
#include "Data/Skeleton.h"
#include "Data/ModelData.h"

using namespace Microsoft::WRL;

const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix; //位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix; //法線用
};

struct SkinCluster {
	static int sSkinClusterNum;
	static const int kSkinClusterMaxNum = 1000;
	static std::map<int, bool> isAlive;
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
	int index;
};

SkinCluster CreateSkinCluster(const Skeleton& skeleton, const ModelData& modeldata);

void ClearSkinCluster(SkinCluster& skinCluster);