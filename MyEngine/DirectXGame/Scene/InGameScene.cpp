#include "InGameScene.h"

InGameScene::InGameScene() {}

InGameScene::~InGameScene() {}

void InGameScene::Finalize() {}

void InGameScene::Initialize() {
	sceneNo_ = INGAME;

	//基本機能
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();
	input_ = InputManager::GetInstance();
	audioManager_ = AudioManager::GetInstance();
	randomManager_ = RandomManager::GetInstance();
	postEffectManager_ = PostEffectManager::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();
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
	lightObj_ = LightObjectManager::GetInstance();
	lightObj_->InitData();

	//影
	shadow_ = std::make_unique<Shadow>();
	shadow_->Initialize();

	//コリジョンマネージャー
	collisionManager_->Initialize();

	//ブレンドモード
	blendMode_ = kBlendModeNormal;

	//画像読み込み
	monsterBallHandle_ = TextureManager::Load("monsterBall.png");
	fenceHandle_ = TextureManager::Load("fence.png");
	skyboxHandle_ = TextureManager::Load("rostock_laage_airport_4k.dds");

	//ゲームオブジェクト
	testParticle1_ = std::make_unique<TestParticle>();
	testParticle1_->Initialize();
	
	//yukariModel_ = Model::Create("yukari", "yukari.obj");
	yukariModel_ = Model::Create("yukariGLTF", "yukariGLTF.gltf");
	yukariModelInfo_.Initialize();
	yukariModelInfo_.SetModel(yukariModel_.get());
	yukariModelInfo_.materialInfo_.material_->enableLightint = true;
	yukariModelInfo_.materialInfo_.environmentTextureHandle_ = skyboxHandle_;

	walkModel_ = Model::Create("human", "walk.gltf");
	sneakWalkModel_ = Model::Create("human", "sneakWalk.gltf");
	boxModel_ = Model::Create("AnimatedCube", "AnimatedCube.gltf");

	walkModelInfo_.Initialize();
	walkModelInfo_.materialInfo_.material_->enableLightint = true;
	walkModelInfo_.materialInfo_.material_->isEnvironment = true;
	walkModelInfo_.materialInfo_.environmentTextureHandle_ = skyboxHandle_;
	walkModelInfo_.SetModel(walkModel_.get());
	walkModelInfo_.SetAnimation(walkModel_->GetAnimationData());

	boxModelInfo_.Initialize();
	boxModelInfo_.materialInfo_.environmentTextureHandle_ = skyboxHandle_;
	boxModelInfo_.SetModel(boxModel_.get());
	boxModelInfo_.SetAnimation(boxModel_->GetAnimationData());

	skybox_ = SkyBox::Create(skyboxHandle_);
	skyboxInfo_.Initialize();
	skyboxInfo_.worldTransform_.data_.scale_ *= 100;

	sprite_ = Sprite::Create();
	spriteInfo_.Initialize(uvCheckerHandle_);

	levelScene_.Initialize("test.json");
}

void InGameScene::Update() {

	//カメラの更新
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::Checkbox("UseDebugCamera", &isDebugCamera_);
	ImGui::End();
#endif // _DEBUG

	if (isDebugCamera_) {
		debugCamera_->Update();
		mainCamera_->Update(debugCamera_->GetWorldTransrom(), debugCamera_->GetWorldMatrix(), debugCamera_->GetProjectionMatrix());
	}
	else {
		gameCamera_->Update();
		mainCamera_->Update(gameCamera_->GetWorldTransrom(), gameCamera_->GetWorldMatrix(), gameCamera_->GetProjectionMatrix());
	}
	//スプライトカメラの更新
	spriteCamera_->Update();
	//ライトの更新
	lightObj_->Update();
	//影の更新
	shadow_->Update(lightObj_->GetDirectionalLightData(0).direction);

	//パーティクルの更新
	testParticle1_->Update();

	levelScene_.Update();

	collisionManager_->Update();
#ifdef _DEBUG

	ImGui::Begin("RenderItemInfo");
	ImGui::BeginTabBar("RenderItemInfo");
	ImGuiManager::GetInstance()->RenderItemDebug("yukariModel", yukariModelInfo_);
	ImGuiManager::GetInstance()->RenderItemDebug("human", walkModelInfo_);
	ImGuiManager::GetInstance()->RenderItemDebug("boxModel", boxModelInfo_);
	ImGuiManager::GetInstance()->RenderItemDebug("skybox", skyboxInfo_);
	ImGuiManager::GetInstance()->SpriteItemDebug("sprite", spriteInfo_);
	ImGui::EndTabBar();
	ImGui::End();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen" };
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();

	ImGui::Begin("PostEffect");
	const char* postEffects[] = { "None", "Copy", "HSVFilter", "GrayScale", "SepiaScale", "Vignette", "Smoothing"};
	int postEffect = postEffectManager_->GetPostEffect();
	ImGui::Combo("postEffect", &postEffect, postEffects, IM_ARRAYSIZE(postEffects));
	postEffectManager_->SetPostEffect(static_cast<PostEffect>(postEffect));

	ImGui::BeginTabBar("PostEffectState");
	if (ImGui::BeginTabItem("HSVFilter")) {
		ImGui::SliderFloat("hue", &postEffectManager_->GetHSVMaterial()->hue, -1.0f, 1.0f);
		ImGui::SliderFloat("saturation", &postEffectManager_->GetHSVMaterial()->saturation, -1.0f, 1.0f);
		ImGui::SliderFloat("value", &postEffectManager_->GetHSVMaterial()->value, -1.0f, 1.0f);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Smoothing")) {
		int32_t kernelSize = postEffectManager_->GetKernelSize();
		ImGui::SliderInt("size", &kernelSize, 1, 5);
		postEffectManager_->SetKernelSize(kernelSize);

		const char* types[] = { "Box", "Gauss" };
		int32_t type = postEffectManager_->GetSmoothingType();
		ImGui::Combo("SmoothingType", &type, types, IM_ARRAYSIZE(types));
		postEffectManager_->SetSmoothingType(type);

		float strength = postEffectManager_->GetBlurStrength();
		ImGui::SliderFloat("Strength", &strength, 1.0f, 5.0f);
		postEffectManager_->SetBlurStrength(strength);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

	ImGui::End();

#endif // _DEBUG
}

void InGameScene::Draw() {

	///背景スプライトの描画開始 

	

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始

	sprite_->Draw(spriteInfo_);

	///前面スプライトの描画終了

	///オブジェクトの描画開始

	yukariModel_->Draw(yukariModelInfo_);
	walkModel_->Draw(walkModelInfo_);
	//boxModel_->Draw(boxModelInfo_);
	//skybox_->Draw(skyboxInfo_);

	//levelScene_.Draw();
	//collisionManager_->Draw();

	///オブジェクトの描画終了

	///パーティクルの描画

	//testParticle1_->Draw();

	///パーティクルの描画終了
}