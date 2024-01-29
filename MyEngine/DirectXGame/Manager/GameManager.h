#pragma once
#include <memory>
#include "DirectXGame/Base/Framework/Framework.h"
#include "DirectXGame/Object/Sprite.h"
#include "DirectXGame/Manager/SceneManager.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"

class GameManager : public Framework
{
public:

	static GameManager* GetInstance();

	void Initialize() override;

	void Finalize() override;

	int Run();

private:
	GameManager() = default;
	~GameManager() = default;

	//メインカメラ
	MainCamera* mainCamera_ = nullptr;

	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

	//FPSの計測
	double deltaTime_;
	double fps_;
	int frameCount_;
	double totalTime_;
};