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
				collider0->isContact_[i] = false;
			}
			for (auto& collider1 : colliders_) {

				std::visit([&](auto& shape0) {
					std::visit([&](auto& shape1) {
						if (IsCollision(shape0, shape1)) {
							collider0->isContact_[collider1->tag_] = true;
						}
						}, collider1->colliderShape_);
				}, collider0->colliderShape_);
			}
		}
	}
}

void CollisionManager::Draw() {

#ifdef _DEBUG
	for (auto& collider : colliders_) {

		if (collider->isDrawCollider_) {
			collider->renderItem_.worldTransform_.data_.translate_ = collider->combinedPosition;
			collider->renderItem_.worldTransform_.data_.rotate_ = collider->combinedRotation;
			collider->renderItem_.worldTransform_.data_.scale_ = collider->combinedScale;
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