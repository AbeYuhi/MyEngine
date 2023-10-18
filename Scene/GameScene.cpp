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

	Vector3 trianglePos[3] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f} };
	triangle_ = Triangle::Create();
	triangle2_ = Triangle::Create(trianglePos);
}

void GameScene::Update() {
	//カメラの更新
	debugCamera_->Update();

	triangle_->Update();
	triangle2_->Update();

	ImGui::ShowDemoWindow();
}

void GameScene::Draw() {

	//三角形
	Triangle::PreDraw(directXCommon_->GetCommandList());

	triangle_->Draw(debugCamera_->GetViewProjectionMatrix(), UVCHECKER);
	triangle2_->Draw(debugCamera_->GetViewProjectionMatrix(), UVCHECKER);

	Triangle::PostDraw();

}