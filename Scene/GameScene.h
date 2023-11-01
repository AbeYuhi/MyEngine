#pragma once
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Manager/ImGuiManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Triangle.h"
#include "../Object/Sprite.h"
#include "../Object/Sphere.h"
#include "../Object/Model.h"
#include "../GameObject/Camera/DebugCamera.h"
#include "../GameObject/Camera/SpriteCamera.h"
#include "../GameObject/Light/DirectionalLight.h"

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
	InputManager* input_;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	//スプライト用カメラ
	std::unique_ptr<SpriteCamera> spriteCamera_;
	//平行ライト
	std::unique_ptr<DirectionalLight> directionalLight_;

	std::unique_ptr<Model> model_;
	RenderItem modelRenderInfo_;
};