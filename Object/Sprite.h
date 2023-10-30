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

class Sprite
{
public: //静的メンバ関数

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	static void PostDraw();

	static std::unique_ptr<Sprite> Create(Vector2 spriteSize = {640, 360});

private: //静的メンバ変数
	static ID3D12GraphicsCommandList* sCommandList_;
	const static UINT kVertexNumber = 4;
	const static UINT kIndexNumber = 6;

public: //メンバ関数
	Sprite();
	~Sprite();

	void Initialize(Vector2 spriteSize);

	void Update();

	void Draw(Matrix4x4 viewProjectionMatrix, UINT textureName = UVCHECKER);

public: //ゲッターセッター

	//inline void SetSize(Vector2 spriteSize) {
	//	//1枚目の三角形
	//	vertexData_[0].position = { 0.0f, spriteSize.y, 0.0f, 1.0f }; //左下
	//	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f }; //左上
	//	vertexData_[2].position = { spriteSize.x, spriteSize.y, 0.0f, 1.0f }; //右下
	//	//2枚目の三角形
	//	vertexData_[3].position = { 0.0f, 0.0f, 0.0f, 1.0f }; //左上
	//	vertexData_[4].position = { spriteSize.x, 0.0f, 0.0f, 1.0f }; //右上
	//	vertexData_[5].position = { spriteSize.x, spriteSize.y, 0.0f, 1.0f }; //右下; }
	//}

	//inline void SetTexCoord(Vector2 texcoord, int index) { vertexData_[index].texcoord = texcoord; }
	inline void SetColor(Vector4 color) { materialData_->color = color; }

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	ComPtr<ID3D12Resource> transformMatrixResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//オブジェクトのローカル情報
	WorldTransform transform_;
	VertexData* vertexData_;
	uint32_t* indexData_;
	Material* materialData_;
	TransformData uvTransform_;
};