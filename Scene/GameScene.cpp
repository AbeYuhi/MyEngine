#include "GameScene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {

}

void GameScene::Initialize() {

	//基本機能の初期化
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();

	triangle_ = new Triangle();
	triangle_->Initialize();
}

void GameScene::Update() {

}

void GameScene::Draw() {

	//三角形
	Triangle::PreDraw(directXCommon_->GetCommandList());

	triangle_->Draw();

	Triangle::PostDraw();

}