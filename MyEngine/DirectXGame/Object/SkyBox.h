#pragma once
#include <memory>
#include <string>
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/VertexData.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/Material.h"
#include "DirectXGame/Data/MaterialInfo.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

class SkyBox
{
public: //制的メンバ関数

	static std::shared_ptr<SkyBox> Create(uint32_t textureHandle);

private: //静的メンバ変数
	const static UINT kVertexNumber = 24;
	const static UINT kIndexNumber = 36;

public: //メンバ関数
	SkyBox();
	~SkyBox();

	void Initialize(uint32_t textureHandle);

	void Draw(RenderItem& renderItem);

	inline void SetSkyBoxTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }
	inline uint32_t GetSkyBoxTextureHandle() { return textureHandle_; }

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> indexResource_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//オブジェクトのローカル情報
	VertexData* vertexData_;
	uint32_t* indexData_;
};

