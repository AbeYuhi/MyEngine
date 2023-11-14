#include "InGameScene.h"

InGameScene::InGameScene() {

}

InGameScene::~InGameScene() {
}

void InGameScene::Initialize() {
	sceneNo_ = INGAME;

	//デバックモード中ならdebugカメラを有効に
	isDebugCamera_ = debugMode_;

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
	testParticle_ = std::make_unique<TestParticle>(&viewProjectionMatrix_, 10);
	testParticle_->Initialize();
}

void InGameScene::Update() {
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

	testParticle_->Update();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen"};
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();
}

void InGameScene::Draw() {
	testParticle_->Draw();
}