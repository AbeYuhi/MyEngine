#include "Collider.h"

void Collider::Initialize(Vector3* translate, Vector3 colliderScale, ColliderTag tag, Vector3* velocity, bool isCollision, bool isDrawCollider) {
	translate_ = translate;
	colliderScale_ = colliderScale;
	tag_ = tag;
	velocity_ = velocity;
	isCollision_ = isCollision;
	isDrawCollider_ = isDrawCollider;
	renderItem_.Initialize();
	isContact_ = false;
	isUnderHit_ = false;
	isTopHit_ = false;
	isLeftHit_ = false;
	isRightHit_ = false;
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
