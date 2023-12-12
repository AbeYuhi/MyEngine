#include "InGameScene.h"

InGameScene::InGameScene() {

}

InGameScene::~InGameScene() {

}

void InGameScene::Finalize() {}

void InGameScene::Initialize() {
	sceneNo_ = INGAME;

	//基本機能
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();
	input_ = InputManager::GetInstance();;
	audioManager_ = AudioManager::GetInstance();
	randomManager_ = RandomManager::GetInstance();
	mainCamera_ = MainCamera::GetInstance();
	spriteCamera_ = SpriteCamera::GetInstance();

	//デバックモード中ならdebugカメラを有効に
	isDebugCamera_ = debugMode_;

	//インゲームカメラ
	gameCamera_ = std::make_unique<InGameCamera>();
	gameCamera_->Initialize();
	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	//スプライトカメラの初期化
	spriteCamera_->Initialize();

	//平行光源
	lightObj_ = std::make_unique<LightObject>();
	lightObj_->Initialize();

	//ブレンドモード
	blendMode_ = kBlendModeNormal;

	//画像読み込み
	monsterBallHandle_ = TextureManager::Load("monsterBall.png");
	fenceHandle_ = TextureManager::Load("fence.png");

	//ゲームオブジェクト
	testParticle1_ = std::make_unique<TestParticle>(100);
	testParticle1_->Initialize();

	groundModel_ = Model::Create("terrain");
	groundModelInfo_.Initialize(false);
	groundModelInfo_.materialInfo_.material_->enableLightint = true;

	monsterBall_ = Sphere::Create();
	monsterBallInfo_.Initialize(false);
	monsterBallInfo_.materialInfo_.material_->enableLightint = true;

	float hatibunnkatu = 1.0f / 8.0f;
	sprite_ = Sprite::Create({ 640, 360 }, uvCheckerHandle_);
	spriteInfo_.Initialize(true);

	windowPos_ = { 0, 100 };
}

void InGameScene::Update() {
	//カメラの更新
	ImGui::Begin("Debug");
	ImGui::Checkbox("UseDebugCamera", &isDebugCamera_);
	ImGui::End();
	
	if (isDebugCamera_) {
		debugCamera_->Update();
		mainCamera_->Update(debugCamera_->GetWorldMatrix(), debugCamera_->GetProjectionMatrix());
	}
	else {
		gameCamera_->Update();
		mainCamera_->Update(gameCamera_->GetWorldMatrix(), gameCamera_->GetProjectionMatrix());
	}
	//スプライトカメラの更新
	spriteCamera_->Update();
	//ライトの更新
	lightObj_->Update();

	//パーティクルの更新
	testParticle1_->Update();

	ImGui::BeginTabBar("RenderItemInfo");
	if (ImGui::BeginTabItem("monsterBall")) {
		ImGui::SliderFloat3("pos", &monsterBallInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &monsterBallInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &monsterBallInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &monsterBallInfo_.materialInfo_.material_->shininess, 0, 100);
		bool a = false;
		if (ImGui::Checkbox("isSpecularReflection", &a)) {
			monsterBallInfo_.materialInfo_.material_->isSpecularReflection = !monsterBallInfo_.materialInfo_.material_->isSpecularReflection;
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("groundModel")) {
		ImGui::SliderFloat3("pos", &groundModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &groundModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &groundModelInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &groundModelInfo_.materialInfo_.material_->shininess, 0, 100);
		bool b = false;
		if (ImGui::Checkbox("isSpecularReflection", &b)) {
			groundModelInfo_.materialInfo_.material_->isSpecularReflection = !groundModelInfo_.materialInfo_.material_->isSpecularReflection;
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("sprite")) {
		ImGui::SliderFloat3("pos", &spriteInfo_.worldTransform_.data_.translate_.x, -1000, 1000);
		ImGui::SliderFloat3("rotate", &spriteInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &spriteInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

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
	//カメラの転送
	mainCamera_->Draw();
	//ライティングの転送
	lightObj_->Draw();

	///背景スプライトの描画開始

	sprite_->Draw(spriteInfo_);

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始



	///前面スプライトの描画終了

	///オブジェクトの描画開始

	//monsterBall_->Draw(monsterBallInfo_, monsterBallHandle_);
	//groundModel_->Draw(groundModelInfo_);
	//testParticle1_->EmitterDraw();

	///オブジェクトの描画終了

	///パーティクルの描画

	//testParticle1_->Draw();

	///パーティクルの描画終了
}