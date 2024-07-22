#pragma once
#include "DirectXGame/Math/AABB.h"
#include "DirectXGame/Math/OBB.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/RenderItem.h"

enum ColliderTag {
	BULLET,
	WALL,
	BUTTON,
	kNumColliderTag,
};

enum ColliderType {
	kAABB,
	kOBB,
	kSPHERE
};

struct Collider {
	union {
		AABB aabb_;
		OBB obb_;
	};
	EulerTransformData* objData_;
	EulerTransformData colliderData_;
	Matrix4x4 worldMatrix_;
	Vector3 scale_;
	Vector3* velocity_;
	Vector3 contactPoint_;
	ColliderTag tag_;
	ColliderType type_;

#ifdef _DEBUG
	RenderItem renderItem_;
#endif // _DEBUG

	bool isContact_[kNumColliderTag];

	//押しつぶされているか
	bool isPush_;

	//Colliderを消すか
	bool isDelete_;

	//衝突判定を描画するか
	bool isDrawCollider_;

	bool isCollisionCheck_;
	
	void Initialize(EulerTransformData* objData, EulerTransformData colliderData, ColliderTag tag, ColliderType type, bool isCollisionCheck, Vector3* velocity = nullptr, bool isDrawCollider = true);
	void Update();
};