#pragma once
#include "DirectXGame/Object/Model.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/Collider.h"

struct StageBlock {
	std::shared_ptr<Model> stageModel;
	RenderItem stageModelInfo;
	Vector3 velocity_;
	std::unique_ptr<Collider> collider;
	int32_t blockInfo;
	bool isBreak;
};