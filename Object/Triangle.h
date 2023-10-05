#pragma once
#include "../DirectXCommon/DirectXCommon.h"
#include "../Math/Vector4.h"

class Triangle
{
public: //静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

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

	void Draw();

private: //メンバ関数

	/// <summary>
	/// 頂点のメモリ確保
	/// </summary>
	void CreateVertexResource();

private: //メンバ変数
	//VertexResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};