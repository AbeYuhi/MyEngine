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

	//デバックモード中ならdebugカメラを有効に
	isDebugCamera_ = debugMode_;

	//画面に表示する際のVPmatrix
	mainCamera_.Initialize();
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
	lightObj_ = std::make_unique<LightObject>();
	lightObj_->Initialize();

	//ブレンドモード
	blendMode_ = kBlendModeNone;

	//画像読み込み
	monsterBallHandle_ = TextureManager::Load("monsterBall.png");
	fenceHandle_ = TextureManager::Load("fence.png");

	//ゲームオブジェクト
	testParticle1_ = std::make_unique<TestParticle>(mainCamera_.GetPViewProjectionMatrix(), 10);
	testParticle1_->Initialize();

	groundModel_ = Model::Create("terrain");
	groundModelInfo_.Initialize(mainCamera_.GetPViewProjectionMatrix());
	groundModelInfo_.materialInfo_.material_->enableLightint = true;

	monsterBall_ = Sphere::Create();
	monsterBallInfo_.Initialize(mainCamera_.GetPViewProjectionMatrix());
	monsterBallInfo_.materialInfo_.material_->enableLightint = true;

	sprite_ = Sprite::Create({ 1280, 720 }, uvCheckerHandle_);
	spriteInfo_.Initialize(spriteCamera_->GetViewProjectionMatrixPointer());

	//サウンド
	soundHandle_ = audioManager_->SoundLoadWave("Alarm01.wav");
}

void InGameScene::Update() {
	//カメラの更新
	ImGui::Begin("Debug");
	ImGui::Checkbox("UseDebugCamera", &isDebugCamera_);
	ImGui::End();
	
	if (isDebugCamera_) {
		debugCamera_->Update();
		mainCamera_.Update(debugCamera_->GetViewProjectionMatrix(), debugCamera_->GetWorldTransrom().translate_);
	}
	else {
		gameCamera_->Update();
		mainCamera_.Update(gameCamera_->GetViewProjectionMatrix(), gameCamera_->GetWorldTransrom().translate_);
	}

	//スプライトカメラの更新
	spriteCamera_->Update();
	//ライトの更新
	lightObj_->Update();

	//パーティクルの更新
	//testParticle1_->Update();

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
	ImGui::EndTabBar();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen"};
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();

	if (input_->IsTriggerKey(DIK_SPACE) && !input_->IsPushKey(DIK_LSHIFT)) {
		if (!audioManager_->IsSoundPlaying(soundHandle_)) {
			audioManager_->SoundPlayWave(soundHandle_, true);
		}
	}

	if (input_->IsPushKey(DIK_SPACE) && input_->IsPushKey(DIK_LSHIFT)) {
		audioManager_->StopLoopWave(soundHandle_);
	}

	if (input_->IsPushKey(DIK_UPARROW)) {
		audioManager_->SetVolume(soundHandle_, 0.5f);
	}

	groundModelInfo_.Update();
	monsterBallInfo_.Update();
	spriteInfo_.Update();
}

void InGameScene::Draw() {
	//カメラの転送
	mainCamera_.Draw();
	//ライティングの転送
	lightObj_->Draw();

	///背景スプライトの描画開始

	//sprite_->Draw(spriteInfo_);

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始



	///前面スプライトの描画終了

	///オブジェクトの描画開始

	//testParticle_->Draw();
	monsterBall_->Draw(monsterBallInfo_, monsterBallHandle_);
	groundModel_->Draw(groundModelInfo_);

	///オブジェクトの描画終了
}