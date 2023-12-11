#pragma once
#include <memory>
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/VertexData.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/Material.h"
#include "DirectXGame/Data/MaterialInfo.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

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
	/// <param name="texSize">画像を切り抜く範囲(0～1)</param>
	/// <param name="isFlipX">横に反転するか</param>
	/// <param name="isFlipY">縦に反転するか</param>
	/// <returns></returns>
	static std::unique_ptr<Sprite> Create(Vector2 spriteSize = { 640, 360 }, uint32_t textureHandle = 0, Vector2 anchorPoint = { 0, 0 }, Vector2 baseUVPos = {0, 0}, Vector2 texSize = {1.0f, 1.0f}, bool isFlipX = false, bool isFlipY = false);

private: //静的メンバ変数
	const static UINT kVertexNumber = 4;
	const static UINT kIndexNumber = 6;

public: //メンバ関数
	Sprite();
	~Sprite();

	void Initialize(Vector2 spriteSize, uint32_t textureHandle, Vector2 anchorPoint, Vector2 baseUVPos, Vector2 texSize, bool isFlipX, bool isFlipY);

	void TransferVertices();

	void Draw(RenderItem& renderItem);

	void Draw(ParticleDrawInfo drawInfo);

public: //ゲッターセッター

	inline uint32_t GetTextureHandle() { return textureHandle_; }
	inline Vector2 GetSize() { return size_; }
	inline Vector2 GetAnchorPoint() { return anchorPoint_; }
	inline Vector2 GetBaseUvPos() { return baseUvPos_; }
	inline Vector2 GetTexSize() { return texSize_; }
	inline bool GetIsFlipX() { return isFlipX_; }
	inline bool GetIsFlipY() { return isFlipY_; }
	inline bool GetIsInvisible() { return isInvisible_; }

	void SetTextureHandle(uint32_t textureHandle);
	void SetSize(Vector2 size);
	void SetAnchorPoint(Vector2 anchorPoint);
	void SetBaseUvPos(Vector2 baseUvPos);
	void SetTexSize(Vector2 texSize);
	void SetIsFlipX(bool isFlip);
	void SetIsFlipY(bool isFlip);
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; }

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
	Vector2 texSize_;
	bool isFlipX_;
	bool isFlipY_;
	bool isInvisible_;
};