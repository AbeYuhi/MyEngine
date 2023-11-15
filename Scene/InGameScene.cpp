#include "InGameScene.h"

InGameScene::InGameScene() {

}

InGameScene::~InGameScene() {
}

void InGameScene::Initialize() {
	sceneNo_ = INGAME;

	//基本機能
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();
	input_ = InputManager::GetInstance();;

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

	//画像読み込み
	monsterBallHandle_ = TextureManager::Load("monsterBall.png");
	fenceHandle_ = TextureManager::Load("fence.png");

	//ゲームオブジェクト
	testParticle_ = std::make_unique<TestParticle>(&viewProjectionMatrix_, 10);
	testParticle_->Initialize();

	groundModel_ = Model::Create("terrain");
	groundModelInfo_.Initialize(&viewProjectionMatrix_);
	groundModelInfo_.materialInfo_.material_->enableLightint = true;

	monsterBall_ = Sphere::Create();
	monsterBallInfo_.Initialize(&viewProjectionMatrix_);
	monsterBallInfo_.materialInfo_.material_->enableLightint = true;

	sprite_ = Sprite::Create({ 1280, 720 });
	spriteInfo_.Initialize(spriteCamera_->GetViewProjectionMatrixPointer());
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

	groundModelInfo_.Update();
	monsterBallInfo_.Update();
	spriteInfo_.Update();
}

void InGameScene::Draw() {
	//ライティングの描画
	directionalLight_->Draw();

	///背景スプライトの描画開始

	//sprite_->Draw(spriteInfo_, uvCheckerHandle_);

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始



	///前面スプライトの描画終了

	///オブジェクトの描画開始

	//testParticle_->Draw();
	//monsterBall_->Draw(monsterBallInfo_, monsterBallHandle_);
	groundModel_->Draw(groundModelInfo_);

	///オブジェクトの描画終了
}