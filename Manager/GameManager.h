#pragma once
#include <memory>
#include "Scene/IScene.h"
#include "Scene/TitleScene.h"
#include "Scene/MenuScene.h"
#include "Scene/InGameScene.h"
#include "Scene/GameOverScene.h"
#include "Scene/GameClearScene.h"
#include "DebugSytem/DebugLog.h"
#include "DebugSytem/ReleseCheck.h"
#include "Base/SafeDelete/SafeDelete.h"
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Manager/TextureManager.h"
#include "Manager/GraphicsPipelineManager.h"
#include "Manager/InputManager.h"
#include "Manager/GameManager.h"
#include "Object/Triangle.h"
#include "Object/Sprite.h"
#include "Object/Sphere.h"

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
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	GameManager* gameManager_ = nullptr;

	//ゲームシーン
	std::unique_ptr<IScene> sceneArr_[GameScene::SCENENUM];
	int sceneNo_;
	int preSceneNo_;
};