#pragma once
#include "MaterialInfo.h"
#include "Transform.h"
#include "SpriteData.h"

struct SpriteItem {
	WorldTransform worldTransform_;
	MaterialInfo materialInfo_;
	SpriteData spriteData_;

	/// <summary>
	/// レンダリング情報の初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="size">スプライトサイズ</param>
	/// <param name="anchorPoint">基準点</param>
	/// <param name="baseUvPos">テクスチャの基準点</param>
	/// <param name="texSize">テクスチャサイズ</param>
	/// <param name="isFlipX">X軸に反転するか</param>
	/// <param name="isFlipY">Y軸に反転するか</param>
	/// <param name="isInvisible">非表示かどうか</param>
	void Initialize(uint32_t textureHandle = 0, Vector2 size = { 640, 360 }, Vector2 anchorPoint = { 0.5f, 0.5f }, Vector2 baseUvPos = { 0, 0 }, Vector2 texSize = { 1.0f, 1.0f }, bool isFlipX = false, bool isFlipY = false) {
		materialInfo_.Initialize();
		worldTransform_.Initialize(true);
		spriteData_.Initialize(textureHandle, size, anchorPoint, baseUvPos, texSize, isFlipX, isFlipY);
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
	}
};

