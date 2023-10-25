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
#include "../Math/Material.h"
#include "../Manager/TextureManager.h"
#include "../Manager/GraphicsPipelineManager.h"

class Sphere
{
public: //静的メンバ関数

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList"></param>
	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	static std::unique_ptr<Sphere> Create();

private: //静的メンバ変数
	static ID3D12GraphicsCommandList* sCommandList_;
	const static UINT kSubDivision = 16;
	const static UINT kVertexNumber = kSubDivision * kSubDivision * 6;

public: //メンバ関数
	Sphere();
	~Sphere();

	void Initialize();

	void Update();

	void Draw(Matrix4x4 viewProjectionMatrix, UINT textureName = UVCHECKER);

public: //ゲッターセッター

	inline Vector3 GetPos() { return transform_.data_.translate_; }
	inline void SetPos(Vector3 pos) { transform_.data_.translate_ = pos; }
	inline Vector3 GetRotate() { return transform_.data_.rotate_; }
	inline void SetRotate(Vector3 rotate) { transform_.data_.rotate_ = rotate; }
	inline Vector3 GetScale() { return transform_.data_.scale_; }
	inline void SetScale(Vector3 scale) { transform_.data_.scale_ = scale; }
	inline void SetTexCoord(Vector2 texcoord, int index) { vertexData_[index].texcoord = texcoord; }
	inline void SetColor(Vector4 color) { materialData_->color = color; }

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	ComPtr<ID3D12Resource> transformMatrixResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	WorldTransform transform_;
	VertexData* vertexData_;
	Material* materialData_;
};