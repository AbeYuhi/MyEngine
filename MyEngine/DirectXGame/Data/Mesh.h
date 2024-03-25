#pragma once
#include <d3d12.h>
#include <stdint.h>
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Data/ModelData.h"
#include "DirectXGame/Data/VertexData.h"
#include "DirectXGame/Data/Bone.h"

struct Mesh {
	//モデルデータ
	ModelData modelData;

	//テクスチャデータ
	uint32_t textureHandle;

	//Boneデータ
	Bone bones;

	//メッシュの名前
	std::string name;

	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource;
	ComPtr<ID3D12Resource> indexResource;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	//オブジェクトのローカル情報
	VertexData* vertexData;
	uint32_t* indexData;
};