#pragma once
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Manager/ImGuiManager.h"
#include "../Object/Triangle.h"
#include "../Object/Sprite.h"
#include "../Object/Sphere.h"
#include "../GameObject/Camera/DebugCamera.h"
#include "../GameObject/Camera/SpriteCamera.h"

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
	//スプライト用カメラ
	std::unique_ptr<SpriteCamera> spriteCamera_;

	std::unique_ptr<Sphere> sphere_;
	TextureName sphereTexture_;
	bool isUseMonsterBall_;

	std::unique_ptr<Sprite> sprite_;
};