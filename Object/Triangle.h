#pragma once
#include <memory>
#include "../Base/CreateResource/CreateResource.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector3_Math.hpp"
#include "../Math/Vector4.h"
#include "../Math/VertexData.h"
#include "../Math/Transform.h"
#include "../Math/Matrix4x4.h"
#include "../Manager/TextureManager.h"
#include "../Manager/ImGuiManager.h"

class Triangle
{
public: //静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	static std::unique_ptr<Triangle> Create(Vector3 pos[3] = sDefaultPos);

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
	//初期位置
	static Vector3 sDefaultPos[3];

public: //メンバ関数
	Triangle();
	~Triangle();

	void Initialize(Vector3 pos[3]);

	void Update();

	void Draw(Matrix4x4 viewProjectionMatrix, UINT textureName = UVCHECKER);

public: //ゲッターセッター

	void SetVertexPos(Vector4 pos, int index) { vertexData_[index].position = pos; }
	void SetTexCoord(Vector2 texcoord, int index) { vertexData_[index].texcoord = texcoord; }
	inline Vector3 GetPos() { return transform_.translate; }
	inline void SetPos(Vector3 pos) { transform_.translate = pos; }
	inline Vector3 GetRotate() { return transform_.rotate; }
	inline void SetRotate(Vector3 rotate) { transform_.rotate = rotate; }
	inline Vector3 GetScale() { return transform_.scale; }
	inline void SetScale(Vector3 scale) { transform_.scale = scale; }
	void SetColor(Vector4 color) { *materialData_ = color; }

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