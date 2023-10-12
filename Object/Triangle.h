#pragma once
#include <memory>
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector3_Math.hpp"
#include "../Math/Vector4.h"
#include "../Math/Transform.h"
#include "../Math/Matrix4x4.h"
#include "../Base/CreateResource/CreateResource.h"

class Triangle
{
public: //静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	static std::unique_ptr<Triangle> Create();

private: //静的メンバ関数

	/// <summary>
	///	ルートシグネチャーの生成
	/// </summary>
	static void CreateRootSignature();

	static void CreatePSO();

private: //静的メンバ変数
	static ComPtr<ID3D12RootSignature> sRootSignature_;
	static ComPtr<ID3D12PipelineState> sGraphicsPipelineState_;
	static D3D12_VIEWPORT sViewPort_;
	static D3D12_RECT sScissorRect_;
	static ID3D12GraphicsCommandList* sCommandList_;
	const static UINT kVertexNumber = 3;

public: //メンバ関数
	Triangle();
	~Triangle();

	void Initialize();

	void Update();

	void Draw(Matrix4x4 viewProjectionMatrix);

private: //メンバ変数
	//VertexResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	WorldTransform transform_;
	Matrix4x4 worldMatrix_;
	Vector4* materialData_;
	Matrix4x4* wvpData_;
};