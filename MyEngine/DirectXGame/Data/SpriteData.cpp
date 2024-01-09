#include "SpriteData.h"

void SpriteData::Initialize(uint32_t textureHandle, Vector2 size, Vector2 anchorPoint, Vector2 baseUvPos, Vector2 texSize, bool isFlipX, bool isFlipY) {
	textureHandle_ = textureHandle;
	size_ = size;
	anchorPoint_ = anchorPoint;
	baseUvPos_ = baseUvPos;
	texSize_ = texSize;
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
}