#pragma once
#include <memory>
#include "../Base/CreateResource/CreateResource.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Manager/ImGuiManager.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector3_Math.hpp"
#include "../Math/Vector4.h"
#include "../Math/Matrix4x4.h"
#include "../Data/VertexData.h"
#include "../Data/Transform.h"
#include "../Data/Material.h"
#include "../Manager/TextureManager.h"
#include "../Manager/GraphicsPipelineManager.h"

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

	void Draw(WorldTransform& transform, UINT textureName = UVCHECKER);

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	TransformData uvTransform_;
	VertexData* vertexData_;
	Material* material_;
};