#pragma once
#include "MaterialInfo.h"
#include "Transform.h"

struct RenderItem {
	WorldTransform worldTransform_;
	MaterialInfo materialInfo_;

	/// <summary>
	/// レンダリング情報の初期化
	/// </summary>
	/// <param name="isSprite">Sprite用のレンダリング情報か</param>
	void Initialize(bool isSprite = false) {
		materialInfo_.Initialize();
		worldTransform_.Initialize(isSprite);
	}

	void Update() {
		materialInfo_.UpdateMatrix();
		worldTransform_.UpdateWorld();
	}
};