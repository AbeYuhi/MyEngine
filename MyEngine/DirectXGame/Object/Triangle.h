#pragma once
#include <memory>
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector3_Math.hpp"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Data/VertexData.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/Material.h"
#include "DirectXGame/Data/MaterialInfo.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

class Triangle
{
public: //静的メンバ関数
	static std::unique_ptr<Triangle> Create(Vector3 pos[3] = sDefaultPos);

private: //静的メンバ変数
	const static UINT kVertexNumber = 3;
	//初期位置
	static Vector3 sDefaultPos[3];

public: //メンバ関数
	Triangle();
	~Triangle();

	void Initialize(Vector3 pos[3]);

	void Draw(RenderItem& renderItem, uint32_t textureHandle);

public: //ゲッターセッター

	void SetVertexPos(Vector4 pos, int index) { vertexData_[index].position = pos; }

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	VertexData* vertexData_;
};