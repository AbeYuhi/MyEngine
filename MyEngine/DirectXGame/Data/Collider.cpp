#include "Collider.h"

void Collider::Initialize(EulerTransformData* objData, EulerTransformData colliderData, ColliderTag tag, ColliderType type, bool isCollisionCheck, Vector3* velocity, bool isDrawCollider) {
	objData_ = objData;
	colliderData_ = colliderData;
	worldMatrix_ = MakeAffineMatrix(colliderData_);
	worldMatrix_ = Multiply(worldMatrix_, MakeAffineMatrix(*objData_));
	tag_ = tag;
	type_ = type;
	velocity_ = velocity;
	isDrawCollider_ = isDrawCollider;
	isCollisionCheck_ = isCollisionCheck;

	//デバック時にコライダーの衝突範囲を可視化するためにrenderItemを初期化する
#ifdef _DEBUG
	renderItem_.Initialize();
#endif // _DEBUG
	for (int i = 0; i < kNumColliderTag; i++) {
		isContact_[i] = false;
	}
	isDelete_ = false;
	isPush_ = false;
}

void Collider::Update() {
	// 合成された位置
	Vector3 combinedPosition = objData_->translate_ + objData_->rotate_ * colliderData_.translate_;
	// 合成された回転
	Vector3 combinedRotation = objData_->rotate_ * colliderData_.rotate_;
	// 合成された大きさ
	Vector3 combinedScale = objData_->scale_ * colliderData_.scale_;

	switch (type_)
	{
	case kAABB:
		aabb_.min.x = combinedPosition.x - (scale_.x / 2.0f);
		aabb_.min.y = combinedPosition.y - (scale_.y / 2.0f);
		aabb_.min.z = combinedPosition.z - (scale_.z / 2.0f);

		aabb_.max.x = combinedPosition.x + (scale_.x / 2.0f);
		aabb_.max.y = combinedPosition.y + (scale_.y / 2.0f);
		aabb_.max.z = combinedPosition.z + (scale_.z / 2.0f);

		ControlMinMax(aabb_);
		break;
	case kOBB:
		break;
	case kSPHERE:
		break;
	default:
		break;
	}
}
