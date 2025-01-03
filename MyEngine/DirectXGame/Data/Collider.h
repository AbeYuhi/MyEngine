#pragma once
#include <variant>
#include "DirectXGame/Math/AABB.h"
#include "DirectXGame/Math/OBB.h"
#include "DirectXGame/Math/Sphere.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix3x3.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/RenderItem.h"
#include "Manager/ImGuiManager.h"

enum ColliderTag {
	BULLET,
	WALL,
	BUTTON,
	LDOOR,
	RDOOR,
	kNumColliderTag,
};

enum ColliderType {
	kAABB,
	kOBB,
	kSPHERE
};

struct Collider {
	EulerTransformData* objData_;
	EulerTransformData colliderData_;
	Matrix4x4 worldMatrix_;
	Vector3 scale_;
	Vector3* velocity_;
	Vector3 contactPoint_;
	Vector3 normalVector_;
	ColliderTag tag_;
	ColliderType type_;
	std::variant<AABB, OBB, Sphere> colliderShape_;

	//合成された位置
	Vector3 combinedPosition;
	// 合成された回転
	Vector3 combinedRotation;
	// 合成された大きさ
	Vector3 combinedScale;

	//衝突した場合の衝突した面の法線
	Vector3 normal;

	RenderItem renderItem_;

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