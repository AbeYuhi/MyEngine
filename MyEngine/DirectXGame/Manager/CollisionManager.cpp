#include "CollisionManager.h"

CollisionManager* CollisionManager::GetInstance() {
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::Initialize() {
	colliders_.clear();

	wireFrame_ = WireFrameBox::Create();
}

void CollisionManager::Update() {

	//コライダーを破壊
	SyncColliderList();

	//AABBの更新
	for (auto& collider : colliders_) {
		collider->Update();
	}

	//衝突判定
	for (auto& collider0 : colliders_) {
		if (collider0->isCollisionCheck_) {
			for (int i = 0; i < kNumColliderTag; i++) {
				collider0->collision_[i].isContact_ = false;
				collider0->collision_[i].isUnderHit_ = false;
				collider0->collision_[i].isTopHit_ = false;
				collider0->collision_[i].isLeftHit_ = false;
				collider0->collision_[i].isRightHit_ = false;
				collider0->collision_[i].isFrontHit_ = false;
				collider0->collision_[i].isBackHit_ = false;
				collider0->collision_[i].isTopLeftFrontHit_ = false;
				collider0->collision_[i].isTopRightFrontHit_ = false;
				collider0->collision_[i].isUnderLeftFrontHit_ = false;
				collider0->collision_[i].isUnderRightFrontHit_ = false;
				collider0->collision_[i].isTopLeftBackHit_ = false;
				collider0->collision_[i].isTopRightBackHit_ = false;
				collider0->collision_[i].isUnderLeftBackHit_ = false;
				collider0->collision_[i].isUnderRightBackHit_ = false;
			}
			for (auto& collider1 : colliders_) {

				if (IsCollision(collider0->aabb_, collider1->aabb_) && collider0 != collider1) {
					collider0->collision_[collider1->tag_].isContact_ = true;
					//八隅でどこが当たっているかの判定
					//左上奥
					if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.min.x, collider0->aabb_.max.y, collider0->aabb_.max.z })) {
						float xDifference = std::abs(collider0->aabb_.min.x - collider1->aabb_.max.x);
						float yDifference = std::abs(collider0->aabb_.max.y - collider1->aabb_.min.y);
						float zDifference = std::abs(collider0->aabb_.max.z - collider1->aabb_.min.z);
						collider0->collision_[collider1->tag_].isTopLeftBackHit_ = true;

						if (Vector3{ collider0->aabb_.min.x, collider0->aabb_.max.y, collider0->aabb_.max.z } == Vector3{ collider1->aabb_.max.x, collider1->aabb_.min.y, collider1->aabb_.min.y }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isTopHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isLeftHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.min.z - (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isBackHit_ = true;
						}
					}
					//右上奥
					else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.max.x, collider0->aabb_.max.y, collider0->aabb_.max.z })) {
						float xDifference = std::abs(collider0->aabb_.max.x - collider1->aabb_.min.x);
						float yDifference = std::abs(collider0->aabb_.max.y - collider1->aabb_.min.y);
						float zDifference = std::abs(collider0->aabb_.max.z - collider1->aabb_.min.z);
						collider0->collision_[collider1->tag_].isTopRightBackHit_ = true;

						if (Vector3{ collider0->aabb_.max.x, collider0->aabb_.max.y, collider0->translate_->z } == Vector3{ collider1->aabb_.min.x, collider1->aabb_.min.y, collider1->translate_->z }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isTopHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isRightHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.min.z - (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isBackHit_ = true;
						}
					}
					//左下奥
					else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.min.x, collider0->aabb_.min.y, collider0->aabb_.max.z })) {
						float xDifference = std::abs(collider0->aabb_.min.x - collider1->aabb_.max.x);
						float yDifference = std::abs(collider0->aabb_.min.y - collider1->aabb_.max.y);
						float zDifference = std::abs(collider0->aabb_.max.z - collider1->aabb_.min.z);
						collider0->collision_[collider1->tag_].isUnderLeftBackHit_ = true;

						if (Vector3{ collider0->aabb_.min.x, collider0->aabb_.min.y, collider0->translate_->z } == Vector3{ collider1->aabb_.max.x, collider1->aabb_.max.y, collider1->translate_->z }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isUnderHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isLeftHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.min.z - (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isBackHit_ = true;
						}
					}
					//右下奥
					else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.max.x, collider0->aabb_.min.y, collider0->aabb_.max.z })) {
						float xDifference = std::abs(collider0->aabb_.max.x - collider1->aabb_.min.x);
						float yDifference = std::abs(collider0->aabb_.min.y - collider1->aabb_.max.y);
						float zDifference = std::abs(collider0->aabb_.max.z - collider1->aabb_.min.z);
						collider0->collision_[collider1->tag_].isUnderRightBackHit_ = true;

						if (Vector3{ collider0->aabb_.max.x, collider0->aabb_.min.y, collider0->translate_->z } == Vector3{ collider1->aabb_.min.x, collider1->aabb_.max.y, collider1->translate_->z }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isUnderHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isRightHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.min.z - (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isBackHit_ = true;
						}
					}

					//左上前
					else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.min.x, collider0->aabb_.max.y, collider0->aabb_.min.z })) {
						float xDifference = std::abs(collider0->aabb_.min.x - collider1->aabb_.max.x);
						float yDifference = std::abs(collider0->aabb_.max.y - collider1->aabb_.min.y);
						float zDifference = std::abs(collider0->aabb_.min.z - collider1->aabb_.max.z);
						collider0->collision_[collider1->tag_].isTopLeftFrontHit_ = true;

						if (Vector3{ collider0->aabb_.min.x, collider0->aabb_.max.y, collider0->aabb_.max.z } == Vector3{ collider1->aabb_.max.x, collider1->aabb_.min.y, collider1->aabb_.min.y }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isTopHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isLeftHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.max.z + (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isFrontHit_ = true;
						}
					}
					//右上前
					else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.max.x, collider0->aabb_.max.y, collider0->aabb_.min.z })) {
						float xDifference = std::abs(collider0->aabb_.max.x - collider1->aabb_.min.x);
						float yDifference = std::abs(collider0->aabb_.max.y - collider1->aabb_.min.y);
						float zDifference = std::abs(collider0->aabb_.min.z - collider1->aabb_.max.z);
						collider0->collision_[collider1->tag_].isTopRightFrontHit_ = true;

						if (Vector3{ collider0->aabb_.max.x, collider0->aabb_.max.y, collider0->translate_->z } == Vector3{ collider1->aabb_.min.x, collider1->aabb_.min.y, collider1->translate_->z }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isTopHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isRightHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.max.z + (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isFrontHit_ = true;
						}
					}
					//左下前
					else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.min.x, collider0->aabb_.min.y, collider0->aabb_.min.z })) {
						float xDifference = std::abs(collider0->aabb_.min.x - collider1->aabb_.max.x);
						float yDifference = std::abs(collider0->aabb_.min.y - collider1->aabb_.max.y);
						float zDifference = std::abs(collider0->aabb_.min.z - collider1->aabb_.max.z);
						collider0->collision_[collider1->tag_].isUnderLeftFrontHit_ = true;

						if (Vector3{ collider0->aabb_.min.x, collider0->aabb_.min.y, collider0->translate_->z } == Vector3{ collider1->aabb_.max.x, collider1->aabb_.max.y, collider1->translate_->z }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isUnderHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isLeftHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.max.z + (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isFrontHit_ = true;
						}
					}
					//右下前
					else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.max.x, collider0->aabb_.min.y, collider0->aabb_.min.z })) {
						float xDifference = std::abs(collider0->aabb_.max.x - collider1->aabb_.min.x);
						float yDifference = std::abs(collider0->aabb_.min.y - collider1->aabb_.max.y);
						float zDifference = std::abs(collider0->aabb_.min.z - collider1->aabb_.max.z);
						collider0->collision_[collider1->tag_].isUnderRightFrontHit_ = true;

						if (Vector3{ collider0->aabb_.max.x, collider0->aabb_.min.y, collider0->translate_->z } == Vector3{ collider1->aabb_.min.x, collider1->aabb_.max.y, collider1->translate_->z }) {

						}
						else if (yDifference <= xDifference && yDifference <= zDifference) {
							collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
							collider0->collision_[collider1->tag_].isUnderHit_ = true;
						}
						else if (xDifference <= zDifference) {
							collider0->contactPoint_ = { collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
							collider0->collision_[collider1->tag_].isRightHit_ = true;
						}
						else {
							collider0->contactPoint_ = { collider0->translate_->x, collider0->translate_->y, collider1->aabb_.max.z + (collider0->colliderScale_.z / 2.0f) };
							collider0->collision_[collider1->tag_].isFrontHit_ = true;
						}
					}

					////上の面が当たっているとき
					//else if (collider0->aabb_.max.y >= collider1->aabb_.min.y) {
					//	collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
					//	collider0->collision_[collider1->tag_].isTopHit_ = true;
					//}
					////下の面が当たっているとき
					//else if (collider0->aabb_.min.y >= collider1->aabb_.max.y) {
					//	collider0->contactPoint_ = { collider0->translate_->x, collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f), collider0->translate_->z };
					//	collider0->collision_[collider1->tag_].isUnderHit_ = true;
					//}
					////左の面が当たっているとき
					//else if (collider0->aabb_.min.x >= collider1->aabb_.max.x) {
					//	collider0->contactPoint_ = { collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
					//	collider0->collision_[collider1->tag_].isLeftHit_ = true;
					//}
					////右の面が当たっているとき				
					//else if (collider0->aabb_.max.x >= collider1->aabb_.min.x) {
					//	collider0->contactPoint_ = { collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
					//	collider0->collision_[collider1->tag_].isRightHit_ = true;
					//}
					////前の面が当たっているとき
					//else if (collider0->aabb_.min.z >= collider1->aabb_.max.z) {
					//	collider0->contactPoint_ = { collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
					//	collider0->collision_[collider1->tag_].isLeftHit_ = true;
					//}
					////奥の面が当たっているとき				
					//else if (collider0->aabb_.max.z >= collider1->aabb_.min.z) {
					//	collider0->contactPoint_ = { collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f), collider0->translate_->y, collider0->translate_->z };
					//	collider0->collision_[collider1->tag_].isRightHit_ = true;
					//}
				}
			}

		}
	}
}

void CollisionManager::Draw() {

#ifdef _DEBUG
	for (auto& collider : colliders_) {

		if (collider->isDrawCollider_) {
			collider->renderItem_.worldTransform_.data_.translate_ = *collider->translate_;
			collider->renderItem_.worldTransform_.data_.scale_ = collider->colliderScale_;
			collider->renderItem_.Update();

			wireFrame_->Draw(collider->renderItem_);
		}
	}
#endif // _DEBUG

}

void CollisionManager::SyncColliderList(){
	for (auto it = colliders_.begin(); it != colliders_.end();) {
		Collider* collider = *it;
		if (collider->isDelete_) {
			it = colliders_.erase(it);
		}
		else {
			it++;
		}
	}
}

void CollisionManager::Reset() {
	colliders_.clear();
}