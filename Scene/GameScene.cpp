#include "InGameScene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
}

void GameScene::Initialize() {
	//基本機能の初期化
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();
	input_ = InputManager::GetInstance();

#ifdef _DEBUG
	isDebugCamera_ = true;
#else
	isDebugCamera_ = false;
#endif // _DEBUG

	//画面に表示する際のVPmatrix
	viewProjectionMatrix_ = MakeIdentity4x4();
	//インゲームカメラ
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();
	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	//スプライト専用カメラ
	spriteCamera_ = std::make_unique<SpriteCamera>();
	spriteCamera_->Initialize();

	//平行光源
	directionalLight_ = DirectionalLight::Create();

	//ブレンドモード
	blendMode_ = kBlendModeNone;

	//ゲームオブジェクト
	plane_ = Plane::Create();
	planeInfo_.Initialize(&viewProjectionMatrix_);
}

void GameScene::Update() {
	//カメラの更新
	ImGui::Begin("Debug");
	ImGui::Checkbox("UseDebugCamera", &isDebugCamera_);
	ImGui::End();

	if (isDebugCamera_) {
		debugCamera_->Update();
		viewProjectionMatrix_ = debugCamera_->GetViewProjectionMatrix();
	}
	else {
		gameCamera_->Update();
		viewProjectionMatrix_ = gameCamera_->GetViewProjectionMatrix();
	}

	//スプライトカメラの更新
	spriteCamera_->Update();
	//ライトの更新
	directionalLight_->Update();

	ImGui::Begin("plane");
	ImGui::SliderFloat3("translate", &planeInfo_.worldTransform_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &planeInfo_.worldTransform_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &planeInfo_.worldTransform_.data_.scale_.x, -10, 10);
	ImGui::DragFloat2("UVTransform", &planeInfo_.materialInfo_.uvTransform_.translate_.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &planeInfo_.materialInfo_.uvTransform_.scale_.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &planeInfo_.materialInfo_.uvTransform_.rotate_.z);
	ImGui::ColorEdit4("Color", &planeInfo_.materialInfo_.material_->color.x);
	ImGui::End();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen"};
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();

	planeInfo_.Update();
}

void GameScene::Draw() {
	plane_->Draw(planeInfo_);
}