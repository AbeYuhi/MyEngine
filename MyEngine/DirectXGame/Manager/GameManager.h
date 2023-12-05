#pragma once
#include <memory>
#include "DirectXGame/Scene/IScene.h"
#include "DirectXGame/Scene/TitleScene.h"
#include "DirectXGame/Scene/MenuScene.h"
#include "DirectXGame/Scene/InGameScene.h"
#include "DirectXGame/Scene/GameOverScene.h"
#include "DirectXGame/Scene/GameClearScene.h"
#include "DirectXGame/DebugSytem/DebugLog.h"
#include "DirectXGame/DebugSytem/ReleseCheck.h"
#include "DirectXGame/Base/SafeDelete/SafeDelete.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/AudioManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Manager/InputManager.h"
#include "DirectXGame/Object/Triangle.h"
#include "DirectXGame/Object/Sprite.h"
#include "DirectXGame/Object/Sphere.h"

class GameManager
{
public:
	static GameManager* GetInstance();

	void Initialize();

	int Run();

private:
	GameManager() = default;
	~GameManager() = default;

	//汎用機能
	WinApp* winApp_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	InputManager* inputManager_ = nullptr;
	ImGuiManager* imGuiManager_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	AudioManager* audioManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	GameManager* gameManager_ = nullptr;

	//ゲームシーン
	std::unique_ptr<IScene> sceneArr_[GameScene::SCENENUM];
	int sceneNo_;
	int preSceneNo_;

	//FPSの計測
	double deltaTime_;
	double fps_;
	int frameCount_;
	double totalTime_;
};