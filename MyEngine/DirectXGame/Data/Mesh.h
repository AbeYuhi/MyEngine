#pragma once
#include <d3d12.h>
#include <stdint.h>
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Data/ModelData.h"
#include "DirectXGame/Data/VertexData.h"

struct Mesh {
	//モデルデータ
	ModelData modelData;

	//テクスチャデータ
	uint32_t textureHandle;

	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//オブジェクトのローカル情報
	VertexData* vertexData;
};