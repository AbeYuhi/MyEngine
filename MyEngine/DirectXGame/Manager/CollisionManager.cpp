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
		if (collider0->tag_ == PLAYER) {
			collider0->isContact_ = false;
			collider0->isUnderHit_ = false;
			collider0->isTopHit_ = false;
			collider0->isLeftHit_ = false;
			collider0->isRightHit_ = false;
			collider0->isPush_ = false;
			for (auto& collider1 : colliders_) {
				if (collider1->isCollision_) {

					if (collider1->tag_ == WALL) {
						if (IsCollision(collider0->aabb_, collider1->aabb_)) {
							collider0->isContact_ = true;
							//四隅でどこが当たっているかの判定
							//左上
							if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.min.x, collider0->aabb_.max.y, collider0->translate_->z })) {
								float xDifference = std::abs(collider0->aabb_.min.x - collider1->aabb_.max.x);
								float yDifference = std::abs(collider0->aabb_.max.y - collider1->aabb_.min.y);

								if (Vector3{ collider0->aabb_.min.x, collider0->aabb_.max.y, collider0->translate_->z } == Vector3{ collider1->aabb_.max.x, collider1->aabb_.min.y, collider1->translate_->z }) {

								}
								else if (yDifference <= xDifference) {
									collider0->velocity_->y = 0;
									collider0->translate_->y = collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f);
									collider0->isTopHit_ = true;
								}
								else {
									collider0->velocity_->x = 0;
									collider0->translate_->x = collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f);
									collider0->isLeftHit_ = true;
								}
							}
							//右上
							else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.max.x, collider0->aabb_.max.y, collider0->translate_->z })) {
								float xDifference = std::abs(collider0->aabb_.max.x - collider1->aabb_.min.x);
								float yDifference = std::abs(collider0->aabb_.max.y - collider1->aabb_.min.y);

								if (Vector3{ collider0->aabb_.max.x, collider0->aabb_.max.y, collider0->translate_->z } == Vector3{ collider1->aabb_.min.x, collider1->aabb_.min.y, collider1->translate_->z }) {

								}
								else if (yDifference <= xDifference) {
									collider0->velocity_->y = 0;
									collider0->translate_->y = collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f);
									collider0->isTopHit_ = true;
								}
								else {
									collider0->velocity_->x = 0;
									collider0->translate_->x = collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f);
									collider0->isRightHit_ = true;
								}
							}
							//左下
							else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.min.x, collider0->aabb_.min.y, collider0->translate_->z })) {
								float xDifference = std::abs(collider0->aabb_.min.x - collider1->aabb_.max.x);
								float yDifference = std::abs(collider0->aabb_.min.y - collider1->aabb_.max.y);

								if (Vector3{ collider0->aabb_.min.x, collider0->aabb_.min.y, collider0->translate_->z } == Vector3{ collider1->aabb_.max.x, collider1->aabb_.max.y, collider1->translate_->z }) {

								}
								else if (yDifference <= xDifference) {
									collider0->velocity_->y = 0;
									collider0->translate_->y = collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f);
									collider0->isUnderHit_ = true;
								}
								else {
									collider0->velocity_->x = 0;
									collider0->translate_->x = collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f);
									collider0->isLeftHit_ = true;
								}
							}
							//右下
							else if (IsCollision(collider1->aabb_, Vector3{ collider0->aabb_.max.x, collider0->aabb_.min.y, collider0->translate_->z })) {
								float xDifference = std::abs(collider0->aabb_.max.x - collider1->aabb_.min.x);
								float yDifference = std::abs(collider0->aabb_.min.y - collider1->aabb_.max.y);

								if (Vector3{ collider0->aabb_.max.x, collider0->aabb_.min.y, collider0->translate_->z } == Vector3{ collider1->aabb_.min.x, collider1->aabb_.max.y, collider1->translate_->z }) {

								}
								else if (yDifference <= xDifference) {
									collider0->velocity_->y = 0;
									collider0->translate_->y = collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f);
									collider0->isUnderHit_ = true;
								}
								else {
									collider0->velocity_->x = 0;
									collider0->translate_->x = collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f);
									collider0->isRightHit_ = true;
								}
							}
							//上の辺が当たっているとき
							else if (collider0->aabb_.max.y >= collider1->aabb_.min.y) {
								collider0->velocity_->y = 0;
								collider0->translate_->y = collider1->aabb_.min.y - (collider0->colliderScale_.y / 2.0f);
								collider0->isTopHit_ = true;
							}
							//下の辺が当たっているとき
							else if (collider0->aabb_.min.y >= collider1->aabb_.max.y) {
								collider0->velocity_->y = 0;
								collider0->translate_->y = collider1->aabb_.max.y + (collider0->colliderScale_.y / 2.0f);
								collider0->isUnderHit_ = true;
							}
							//左の辺が当たっているとき
							else if (collider0->aabb_.min.x >= collider1->aabb_.max.x) {
								collider0->velocity_->x = 0;
								collider0->translate_->x = collider1->aabb_.max.x + (collider0->colliderScale_.x / 2.0f);
								collider0->isLeftHit_ = true;
							}
							//右の辺が当たっているとき				
							else if (collider0->aabb_.max.x >= collider1->aabb_.min.x) {
								collider0->velocity_->x = 0;
								collider0->translate_->x = collider1->aabb_.min.x - (collider0->colliderScale_.x / 2.0f);
								collider0->isRightHit_ = true;
							}
						}
					}

				}
			}
		}
	}
}

void CollisionManager::Draw() {

#ifdef _DEBUG
	for (auto& collider : colliders_) {

		if (collider->isDrawCollider_ && collider->isCollision_) {
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