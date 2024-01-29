#include "Player.h"
#include "DirectXGame/GameObject/Map/Stage.h"

Player::Player(){}
Player::~Player(){}

void Player::Initialize() {
	input_ = InputManager::GetInstance();

	isDead_ = false;

	playerModel_ = Model::Create("cube", "cube.obj");
	playerModelInfo_.Initialize();
	playerModelInfo_.worldTransform_.data_.scale_ = { 0.7f, 0.7f, 0.7f };

	collider_ = std::make_unique<Collider>();
	collider_->Initialize(&playerModelInfo_.worldTransform_.data_.translate_, { 0.7f, 0.7f, 0.7f }, PLAYER, &velocity_);
	CollisionManager::GetInstance()->AddCollider(collider_.get());
}

void Player::Update() {

	velocity_.x = 0;
	if (input_->IsPushKey(DIK_A) && !collider_->isLeftHit_) {
		velocity_.x -= 0.1f;
	}
	if (input_->IsPushKey(DIK_D) && !collider_->isRightHit_) {
		velocity_.x += 0.1f;
	}

	if (input_->IsTriggerKey(DIK_SPACE) && collider_->isUnderHit_) {
		velocity_.y = 0.25f;
	}

	if (collider_->isPush_ && collider_->isUnderHit_) {
		isDead_ = true;
	}

	if (!collider_->isUnderHit_) {
		velocity_.y -= 0.5f * (1.0f / 60.0f);
	}

	playerModelInfo_.worldTransform_.data_.translate_ += velocity_;
	playerModelInfo_.Update();
}	

void Player::Draw() {

	playerModel_->Draw(playerModelInfo_);

}
