#pragma once
#include <memory>
#include "Base/CreateResource/CreateResource.h"
#include "Base/DirectXCommon/DirectXCommon.h"
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
#include "Manager/TextureManager.h"
#include "Manager/ImGuiManager.h"
#include "Manager/GraphicsPipelineManager.h"

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

	void Draw(RenderItem& renderItem, UINT textureName = UVCHECKER);

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