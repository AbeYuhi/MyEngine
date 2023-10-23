#pragma once
#include <memory>
#include "../Base/CreateResource/CreateResource.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Manager/ImGuiManager.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector3_Math.hpp"
#include "../Math/Vector4.h"
#include "../Math/VertexData.h"
#include "../Math/Transform.h"
#include "../Math/Matrix4x4.h"
#include "../Manager/TextureManager.h"

class Sphere
{
public: //静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	static std::unique_ptr<Sphere> Create();

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
	const static UINT kSubDivision = 32;
	const static UINT kVertexNumber = kSubDivision * kSubDivision * 6;

public: //メンバ関数
	Sphere();
	~Sphere();

	void Initialize();

	void Update();

	void Draw(Matrix4x4 viewProjectionMatrix, UINT textureName = UVCHECKER);

public: //ゲッターセッター

	inline Vector3 GetPos() { return transform_.translate; }
	inline void SetPos(Vector3 pos) { transform_.translate = pos; }
	inline Vector3 GetRotate() { return transform_.rotate; }
	inline void SetRotate(Vector3 rotate) { transform_.rotate = rotate; }
	inline Vector3 GetScale() { return transform_.scale; }
	inline void SetScale(Vector3 scale) { transform_.scale = scale; }
	inline void SetTexCoord(Vector2 texcoord, int index) { vertexData_[index].texcoord = texcoord; }
	inline void SetColor(Vector4 color) { *materialData_ = color; }

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	WorldTransform transform_;
	Matrix4x4 worldMatrix_;
	VertexData* vertexData_;
	Vector4* materialData_;
	Matrix4x4* wvpData_;
};