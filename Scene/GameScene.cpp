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

	sphere_ = Sphere::Create();

	/*Vector3 triangle2Pos[3] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f} };
	triangle2_ = Triangle::Create(triangle2Pos);*/

	sprite_ = Sprite::Create({620, 360});
}

void GameScene::Update() {
	//カメラの更新
	debugCamera_->Update();
	spriteCamera_->Update();

	sphere_->Update();

	sprite_->Update();
}

void GameScene::Draw() {

	//三角形
	Triangle::PreDraw(directXCommon_->GetCommandList());

	Triangle::PostDraw();

	//球
	Sphere::PreDraw(directXCommon_->GetCommandList());

	sphere_->Draw(debugCamera_->GetViewProjectionMatrix());

	Sphere::PostDraw();

	//スプライト
	Sprite::PreDraw(directXCommon_->GetCommandList());

	sprite_->Draw(spriteCamera_->GetViewProjectionMatrix());

	Sprite::PostDraw();

}