#include "GameScene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
}

void GameScene::Initialize() {
	//基本機能の初期化
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();
	input_ = InputManager::GetInstance();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	spriteCamera_ = std::make_unique<SpriteCamera>();
	spriteCamera_->Initialize();

	directionalLight_ = DirectionalLight::Create();

	/*Vector3 triangle2Pos[3] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f} };
	triangle2_ = Triangle::Create(triangle2Pos);*/

	sprite_ = Sprite::Create({620, 360});
	spriteInfo_.Initialize(spriteCamera_->GetViewProjectionMatrixPointer());

	model_ = Model::Create("bunny");
	modelRenderInfo_.Initialize(debugCamera_->GetViewProjectionMatrixPointer());
}

void GameScene::Update() {
	//カメラの更新
	debugCamera_->Update();
	spriteCamera_->Update();
	//ライトの更新
	directionalLight_->Update();

	ImGui::Begin("modelRenderInfo");
	ImGui::SliderFloat3("translate", &modelRenderInfo_.worldTransform_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &modelRenderInfo_.worldTransform_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &modelRenderInfo_.worldTransform_.data_.scale_.x, -10, 10);
	ImGui::DragFloat2("UVTransform", &modelRenderInfo_.materialInfo_.uvTransform_.translate_.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &modelRenderInfo_.materialInfo_.uvTransform_.scale_.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &modelRenderInfo_.materialInfo_.uvTransform_.rotate_.z);
	if (ImGui::Button("IsLighting")) {
		bool isLighting = modelRenderInfo_.materialInfo_.material_->enableLightint;
		modelRenderInfo_.materialInfo_.material_->enableLightint = !isLighting;
	}
	ImGui::End();

	if (input_->IsMousePush(0)) {
		spriteInfo_.worldTransform_.data_.translate_ += {input_->GetMouseMovement().x, input_->GetMouseMovement().y, 0 };
	}

	spriteInfo_.Update();
	modelRenderInfo_.Update();
}

void GameScene::Draw() {

	sprite_->Draw(spriteInfo_);
	model_->Draw(modelRenderInfo_);

}