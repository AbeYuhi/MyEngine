#pragma once
#include "../WinApp/WinApp.h"
#include "../DirectXCommon/DirectXCommon.h"
#include "../Object/Triangle.h"

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

	Triangle* triangle_;
};