#pragma once
#include <stdint.h>
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"

struct SpriteData {
	uint32_t textureHandle_;
	Vector2 size_;
	Vector2 anchorPoint_;
	Vector2 baseUvPos_;
	Vector2 texSize_;
	bool isFlipX_;
	bool isFlipY_;

	void Initialize(uint32_t textureHandle = 0, Vector2 size = { 640, 360 }, Vector2 anchorPoint = { 0.5f, 0.5f }, Vector2 baseUvPos = { 0, 0 }, Vector2 texSize = { 1.0f, 1.0f }, bool isFlipX = false, bool isFlipY = false);
};