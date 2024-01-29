#pragma once
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/InputManager.h"
#include "DirectXGame/Manager/AudioManager.h"
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Manager/RandomManager.h"
#include "DirectXGame/GameObject/Map/CreateStage.h"
#include "DirectXGame/Data/StageBlock.h"

class Player;

class Stage
{
public:
	Stage();
	~Stage();

	void Initialize(std::string stageFileName, Vector3 size, Player* player);

	void Update();

	void Draw();

	void ResetStage();

public: //ゲッターセッター

	inline std::list<std::unique_ptr<StageBlock>>* GetStageBlock() { return &stageBlocks_; }
	inline void SetPlayer(Player* player) { player_ = player; }

private:
	Player* player_;
	std::string stageFileName_;
	Vector3 size_;
	std::list<std::unique_ptr<StageBlock>> stageBlocks_;

};

