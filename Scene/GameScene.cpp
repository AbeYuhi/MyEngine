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

	triangle_ = Triangle::Create();
}

void GameScene::Update() {
	//カメラの更新
	debugCamera_->Update();

	triangle_->Update();
}

void GameScene::Draw() {

	//三角形
	Triangle::PreDraw(directXCommon_->GetCommandList());

	triangle_->Draw(debugCamera_->GetViewProjectionMatrix());

	Triangle::PostDraw();

}