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

	directionalLight_ = DirectionalLight::Create();

	//スフィア
	sphere_ = Sphere::Create();

	/*Vector3 triangle2Pos[3] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f} };
	triangle2_ = Triangle::Create(triangle2Pos);*/

	sprite_ = Sprite::Create({620, 360});

	model_ = Model::Create("axis");
	modelTransformData_.Initialize(debugCamera_->GetViewProjectionMatrixPointer());
	modelTransformData2_.Initialize(debugCamera_->GetViewProjectionMatrixPointer());
}

void GameScene::Update() {
	//カメラの更新
	debugCamera_->Update();
	spriteCamera_->Update();
	//ライトの更新
	directionalLight_->Update();

	
	ImGui::Begin("modelPos");
	ImGui::SliderFloat3("translate", &modelTransformData_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &modelTransformData_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &modelTransformData_.data_.scale_.x, -10, 10);
	ImGui::End();

	ImGui::Begin("modelPos2");
	ImGui::SliderFloat3("translate", &modelTransformData2_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &modelTransformData2_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &modelTransformData2_.data_.scale_.x, -10, 10);
	ImGui::End();

	modelTransformData_.UpdateWorld();
	modelTransformData2_.UpdateWorld();
}

void GameScene::Draw() {

	//三角形

	//球
	//sphere_->Draw(debugCamera_->GetViewProjectionMatrix(), UVCHECKER);

	//スプライト
	//sprite_->Draw(spriteCamera_->GetViewProjectionMatrix());

	model_->Draw(modelTransformData_);
	model_->Draw(modelTransformData2_);

}