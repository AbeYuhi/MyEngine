#pragma once
#include <memory>
#include "Base/CreateResource/CreateResource.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector3_Math.hpp"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Data/VertexData.h"
#include "Data/Transform.h"
#include "Data/Material.h"
#include "Data/MaterialInfo.h"
#include "Data/RenderItem.h"
#include "Data/ParticleDrawInfo.h"
#include "Manager/TextureManager.h"
#include "Manager/GraphicsPipelineManager.h"

class Plane
{
public: //静的メンバ関数
	static std::unique_ptr<Plane> Create();

private: //静的メンバ変数
	const static UINT kVertexNumber = 4;
	const static UINT kIndexNumber = 6;

public: //メンバ関数
	Plane();
	~Plane();

	void Initialize();

	void Draw(RenderItem& renderItem, uint32_t textureHandle = 0);
	
	void Draw(ParticleDrawInfo drawInfo, uint32_t textureHandle = 0);

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> indexResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//オブジェクトのローカル情報
	VertexData* vertexData_;
	uint32_t* indexData_;
};