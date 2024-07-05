#include "Collider.h"

void Collider::Initialize(Vector3* translate, Vector3 colliderScale, ColliderTag tag, bool isCollisionCheck, Vector3* velocity, bool isDrawCollider) {
	translate_ = translate;
	colliderScale_ = colliderScale;
	tag_ = tag;
	velocity_ = velocity;
	isDrawCollider_ = isDrawCollider;
	isCollisionCheck_ = isCollisionCheck;

	//デバック時にコライダーの衝突範囲を可視化するためにrenderItemを初期化する
#ifdef _DEBUG
	renderItem_.Initialize();
#endif // _DEBUG
	for (int i = 0; i < kNumColliderTag; i++) {
		collision_[i].isContact_ = false;
		collision_[i].isUnderHit_ = false;
		collision_[i].isTopHit_ = false;
		collision_[i].isLeftHit_ = false;
		collision_[i].isRightHit_ = false;
		collision_[i].isFrontHit_ = false;
		collision_[i].isBackHit_ = false;

		collision_[i].isTopLeftFrontHit_ = false;
		collision_[i].isTopRightFrontHit_ = false;
		collision_[i].isUnderLeftFrontHit_ = false;
		collision_[i].isUnderRightFrontHit_ = false;
		collision_[i].isTopLeftBackHit_ = false;
		collision_[i].isTopRightBackHit_ = false;
		collision_[i].isUnderLeftBackHit_ = false;
		collision_[i].isUnderRightBackHit_ = false;
	}
	isDelete_ = false;
	isPush_ = false;
}

void Collider::Update() {

	aabb_.min.x = translate_->x - (colliderScale_.x / 2.0f);
	aabb_.min.y = translate_->y - (colliderScale_.y / 2.0f);
	aabb_.min.z = translate_->z - (colliderScale_.z / 2.0f);

	aabb_.max.x = translate_->x + (colliderScale_.x / 2.0f);
	aabb_.max.y = translate_->y + (colliderScale_.y / 2.0f);
	aabb_.max.z = translate_->z + (colliderScale_.z / 2.0f);

	ControlMinMax(aabb_);
}
