#pragma once
#include <memory>
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/VertexData.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/Material.h"
#include "DirectXGame/Data/MaterialInfo.h"
#include "DirectXGame/Data/SpriteItem.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"
#include "DirectXGame/Data/SpriteData.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

class Sprite
{
public: //静的メンバ関数

	/// <summary>
	/// スプライトの生成
	/// </summary>
	static std::shared_ptr<Sprite> Create();

private: //静的メンバ変数
	const static UINT kVertexNumber = 4;
	const static UINT kIndexNumber = 6;

public: //メンバ関数
	Sprite();
	~Sprite();

	void Initialize();

	void TransferVertices(SpriteData spriteData);

	void Draw(SpriteItem& spriteItem);

	void Draw(ParticleDrawInfo drawInfo, SpriteData spriteData);

public: //ゲッターセッター

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