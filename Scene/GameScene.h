#pragma once
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Manager/ImGuiManager.h"
#include "../Object/Triangle.h"
#include "../DebugSytem/DebugCamera.h"

class GameScene {
public:
	GameScene();
	~GameScene();

	void Initialize();

	void Update();

	void Draw();

private:
	//基本機能
	WinApp* winApp_;
	DirectXCommon* directXCommon_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	std::unique_ptr<Triangle> triangle_;
	std::unique_ptr<Triangle> triangle2_;
};