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

	//sprite_ = Sprite::Create({620, 360});

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

	if (input_->IsPushKey(DIK_W)) {
		modelRenderInfo_.worldTransform_.data_.translate_.y += 0.01f;
	}
	if (input_->IsPushKey(DIK_S)) {
		modelRenderInfo_.worldTransform_.data_.translate_.y -= 0.01f;
	}
	if (input_->IsPushKey(DIK_A)) {
		modelRenderInfo_.worldTransform_.data_.translate_.x -= 0.01f;
	}
	if (input_->IsPushKey(DIK_D)) {
		modelRenderInfo_.worldTransform_.data_.translate_.x += 0.01f;
	}

	modelRenderInfo_.Update();
}

void GameScene::Draw() {

	model_->Draw(modelRenderInfo_);

}