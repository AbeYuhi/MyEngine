#pragma once
#include "DirectXGame/Math/AABB.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/RenderItem.h"

enum ColliderTag {
	PLAYER,
	WALL,
};

struct Collider {
	AABB aabb_;
	Vector3* translate_;
	Vector3* velocity_;
	Vector3 colliderScale_;
	ColliderTag tag_;
	RenderItem renderItem_;

	//衝突判定があるか
	bool isCollision_;

	//衝突判定を描画するか
	bool isDrawCollider_;
	
	//衝突しているか
	bool isContact_;
	bool isUnderHit_;
	bool isTopHit_;
	bool isLeftHit_;
	bool isRightHit_;

	//Colliderを消すか
	bool isDelete_;

	//押しつぶされているか
	bool isPush_;

	void Initialize(Vector3* translate, Vector3 colliderScale, ColliderTag tag, Vector3* velocity = nullptr, bool isCollision = true, bool isDrawCollider = true);
	void Update();
};