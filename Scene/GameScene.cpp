#include "GameScene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
}

void GameScene::Initialize() {
	//基本機能の初期化
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	spriteCamera_ = std::make_unique<SpriteCamera>();
	spriteCamera_->Initialize();

	triangle_ = Triangle::Create();

	Vector3 triangle2Pos[3] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f} };
	triangle2_ = Triangle::Create(triangle2Pos);

	sprite_ = Sprite::Create({620, 360});
}

void GameScene::Update() {
	//カメラの更新
	debugCamera_->Update();
	spriteCamera_->Update();

	triangle_->Update();
	triangle2_->Update();

	sprite_->Update();
}

void GameScene::Draw() {

	//三角形
	Triangle::PreDraw(directXCommon_->GetCommandList());

	triangle_->Draw(debugCamera_->GetViewProjectionMatrix(), UVCHECKER);
	triangle2_->Draw(debugCamera_->GetViewProjectionMatrix(), UVCHECKER);

	Triangle::PostDraw();

	//スプライト
	Sprite::PreDraw(directXCommon_->GetCommandList());

	sprite_->Draw(spriteCamera_->GetViewProjectionMatrix(), UVCHECKER);

	Sprite::PostDraw();

}