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
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

class Sphere
{
public: //静的メンバ関数

	static std::unique_ptr<Sphere> Create();

private: //静的メンバ変数
	const static UINT kSubDivision = 16;
	const static UINT kVertexNumber = kSubDivision * kSubDivision * 6;

public: //メンバ関数
	Sphere();
	~Sphere();

	void Initialize();

	void Draw(RenderItem& renderItem, uint32_t textureHandle);

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	VertexData* vertexData_;
};