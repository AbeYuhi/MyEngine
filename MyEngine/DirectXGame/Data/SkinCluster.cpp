#include "SkinCluster.h"

SkinCluster CreateSkinCluster(const Skeleton& skeleton, const ModelData& modeldata, const ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	sSkinClusterNum++;
	SkinCluster skinCluster;
	//palette用のResource確保
	skinCluster.paletteResource = CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette, skeleton.joints.size() };
	skinCluster.paletteSrvHandle.first = dxCommon->GetCPUDescriptorHandle(2000 + sSkinClusterNum);
	skinCluster.paletteSrvHandle.second = dxCommon->GetGPUDescriptorHandle(2000 + sSkinClusterNum);

	//palette用のsrvを作成

	//influence用のResource確保

	//influence用のVBVを作成

	//InverceBindPoseMatrixの保存領域を作成

	//ModelDataのSkinCluaster情報を解析してInfluenceの中身を埋める

	return skinCluster;
}
