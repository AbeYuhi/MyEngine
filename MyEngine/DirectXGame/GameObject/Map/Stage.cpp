#include "Stage.h"
#include "DirectXGame/GameObject/Entity/Player.h"

Stage::Stage(){}
Stage::~Stage(){}

void Stage::Initialize(std::string stageFileName, Vector3 size, Player* player) {
	player_ = player;
	stageFileName_ = stageFileName;
	size_ = size;

	stageBlocks_.clear();
	stageBlocks_ = CreateStage(stageFileName, size);

	for (auto& stageBlock : stageBlocks_) {
		if (stageBlock->blockInfo == 1) {
			stageBlock->collider = std::make_unique<Collider>();
			stageBlock->collider->Initialize(&stageBlock->stageModelInfo.worldTransform_.data_.translate_, size, WALL);
			CollisionManager::GetInstance()->AddCollider(stageBlock->collider.get());
		}

		if (stageBlock->blockInfo == 9) {
			player->SetPlayerPos(stageBlock->stageModelInfo.worldTransform_.data_.translate_);
		}
	}
}

void Stage::Update() {

	for (auto itBlock = stageBlocks_.begin(); itBlock != stageBlocks_.end();) {
		StageBlock* stageBlock = itBlock->get();

		if (stageBlock->collider) {

		}

		stageBlock->stageModelInfo.Update();

		if (stageBlock->blockInfo == 0) {
			stageBlock->collider->isDelete_ = true;
			CollisionManager::GetInstance()->SyncColliderList();
			itBlock = stageBlocks_.erase(itBlock);
		}
		else {
			itBlock++;
		}
	}
}

void Stage::Draw() {

	for (auto& stageBlock : stageBlocks_) {
		if (stageBlock->blockInfo == 1) {
			stageBlock->stageModel->Draw(stageBlock->stageModelInfo);
		}
		if (stageBlock->blockInfo == 2) {
			stageBlock->stageModel->Draw(stageBlock->stageModelInfo);
		}
		if (stageBlock->blockInfo == 3) {
			stageBlock->stageModel->Draw(stageBlock->stageModelInfo);
		}
		if (stageBlock->blockInfo == 4) {
			stageBlock->stageModel->Draw(stageBlock->stageModelInfo, 1);
		}
		if (stageBlock->blockInfo == 5) {
			stageBlock->stageModel->Draw(stageBlock->stageModelInfo, 2);
		}
	}

}

void Stage::ResetStage() {

	for (auto itBlock = stageBlocks_.begin(); itBlock != stageBlocks_.end(); itBlock++) {
		StageBlock* stageBlock = itBlock->get();
		if (stageBlock->collider) {
			stageBlock->collider->isDelete_ = true;
		}
	}
	CollisionManager::GetInstance()->SyncColliderList();

	stageBlocks_.clear();

	stageBlocks_ = CreateStage(stageFileName_, size_);

	for (auto& stageBlock : stageBlocks_) {
		if (stageBlock->blockInfo == 1) {
			stageBlock->collider = std::make_unique<Collider>();
			stageBlock->collider->Initialize(&stageBlock->stageModelInfo.worldTransform_.data_.translate_, size_, WALL);
			CollisionManager::GetInstance()->AddCollider(stageBlock->collider.get());
		}

		if (stageBlock->blockInfo == 9) {
			player_->SetPlayerPos(stageBlock->stageModelInfo.worldTransform_.data_.translate_);
		}
	}
}