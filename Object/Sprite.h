#pragma once
#include <memory>
#include "Base/CreateResource/CreateResource.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
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
#include "Data/ParticleDrawInfo.h"
#include "Manager/TextureManager.h"
#include "Manager/GraphicsPipelineManager.h"

class Sprite
{
public: //静的メンバ関数

	/// <summary>
	/// スプライトの生成
	/// </summary>
	/// <param name="spriteSize">スプライトのサイズ</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="anchorPoint">スプライトの基準点</param>
	/// <param name="baseUVPos">Texcoordの基準点(0～1)</param>
	/// <param name="isFlipX">横に反転するか</param>
	/// <param name="isFlipY">縦に反転するか</param>
	/// <returns></returns>
	static std::unique_ptr<Sprite> Create(Vector2 spriteSize = { 640, 360 }, uint32_t textureHandle = 0, Vector2 anchorPoint = { 0, 0 }, Vector2 baseUVPos = {0, 0}, bool isFlipX = false, bool isFlipY = false);

private: //静的メンバ変数
	const static UINT kVertexNumber = 4;
	const static UINT kIndexNumber = 6;

public: //メンバ関数
	Sprite();
	~Sprite();

	void Initialize(Vector2 spriteSize, uint32_t textureHandle, Vector2 anchorPoint, Vector2 baseUVPos, bool isFlipX, bool isFlipY);

	void TransferVertices();

	void Draw(RenderItem& renderItem);

	void Draw(ParticleDrawInfo drawInfo);

public: //ゲッターセッター

	void SetTextureHandle(uint32_t textureHandle);
	void SetSize(Vector2 size);
	void SetAnchorPoint(Vector2 anchorPoint);
	void SetBaseUvPos(Vector2 baseUvPos);
	void SetIsFlipX(bool isFlip);
	void SetIsFlipY(bool isFlip);

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> indexResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	//オブジェクトのローカル情報
	VertexData* vertexData_;
	uint32_t* indexData_;
	uint32_t textureHandle_;
	Vector2 size_;
	Vector2 anchorPoint_;
	Vector2 baseUvPos_;
	bool isFlipX_;
	bool isFlipY_;
};