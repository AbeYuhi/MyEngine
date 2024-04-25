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
	input_ = InputManager::GetInstance();
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

	//影
	shadow_ = std::make_unique<Shadow>();
	shadow_->Initialize();

	//ブレンドモード
	blendMode_ = kBlendModeNormal;

	//画像読み込み
	monsterBallHandle_ = TextureManager::Load("monsterBall.png");
	fenceHandle_ = TextureManager::Load("fence.png");

	//ゲームオブジェクト
	testParticle1_ = std::make_unique<TestParticle>();
	testParticle1_->Initialize();

	yukariModel_ = Model::Create("yukari", "yukari.obj");
	yukariModelInfo_.Initialize();
	yukariModelInfo_.materialInfo_.material_->enableLightint = false;
	yukariModelInfo_.worldTransform_.data_.translate_.y = 1;

	groundModel_ = Model::Create("planeGltf", "plane.gltf");
	groundModelInfo_.Initialize();
	groundModelInfo_.materialInfo_.material_->enableLightint = true;


	//cubeModel_ = Model::Create("multiCube", "multiCube.gltf");
	cubeModel_ = Model::Create("AnimatedCube", "AnimatedCube.gltf");
	cubeModelInfo_.Initialize();
	cubeModelInfo_.animation_.SetInialNode(cubeModel_->GetInialNode());
	cubeModelInfo_.animation_.SetAnimation(cubeModel_->GetAnimationData());

	//testModel_ = Model::Create("human", "sneakWalk.gltf");
	testModel_ = Model::Create("human", "walk.gltf");
	testModelInfo_.Initialize();
	testModelInfo_.animation_.SetInialNode(testModel_->GetInialNode());
	testModelInfo_.animation_.SetAnimation(testModel_->GetAnimationData());
	testModelInfo_.animation_.SetSkeleton();

	sphereModel_ = Model::Create("sphere", "sphere.obj");

	sprite_ = Sprite::Create();
	spriteInfo_.Initialize(uvCheckerHandle_);
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
	//影の更新
	shadow_->Update(lightObj_->GetDirectionalLightData(0).direction);

	//パーティクルの更新
	testParticle1_->Update();

#ifdef _DEBUG
	ImGui::BeginTabBar("RenderItemInfo");
	if (ImGui::BeginTabItem("YukariModel")) {
		ImGui::SliderFloat3("pos", &yukariModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &yukariModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &yukariModelInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &yukariModelInfo_.materialInfo_.material_->shininess, 0, 100);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("groundModel")) {
		ImGui::SliderFloat3("pos", &groundModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &groundModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &groundModelInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &groundModelInfo_.materialInfo_.material_->shininess, 0, 100);
		bool b = 0;
		if (groundModelInfo_.materialInfo_.material_->isSpecularReflection == 0) {
			b = false;
		}
		else {
			b = true;
		}
		ImGui::Checkbox("isSpecularReflection", &b);
		groundModelInfo_.materialInfo_.material_->isSpecularReflection = b;
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("cubeModel")) {
		ImGui::SliderFloat3("pos", &cubeModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &cubeModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &cubeModelInfo_.worldTransform_.data_.scale_.x, -10, 10);

		for (auto it = cubeModelInfo_.animation_.infos.begin(); it != cubeModelInfo_.animation_.infos.end(); it++) {
			ImGui::Checkbox(it->data.name.c_str(), &it->isAnimation);
			std::string animationSpeed = it->data.name + ": speed";
			ImGui::SliderFloat(animationSpeed.c_str(), &it->animationSpeed, 0.0f, 5.0f);
		}

		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("humanModel")) {
		ImGui::SliderFloat3("pos", &testModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &testModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &testModelInfo_.worldTransform_.data_.scale_.x, -10, 10);

		for (auto it = testModelInfo_.animation_.infos.begin(); it != testModelInfo_.animation_.infos.end(); it++) {
			ImGui::Checkbox(it->data.name.c_str(), &it->isAnimation);
			std::string animationSpeed = it->data.name + ": speed";
			ImGui::SliderFloat(animationSpeed.c_str(), &it->animationSpeed, 0.0f, 5.0f);
		}

		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("sprite")) {
		ImGui::SliderFloat3("pos", &spriteInfo_.worldTransform_.data_.translate_.x, 0, 1280);
		ImGui::SliderFloat3("rotate", &spriteInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &spriteInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen" };
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();
#endif // _DEBUG

	yukariModelInfo_.Update();
	groundModelInfo_.Update();
	cubeModelInfo_.Update();
	testModelInfo_.Update();
	spriteInfo_.Update();
}

void InGameScene::Draw() {
	//カメラの転送
	mainCamera_->Draw();
	//ライティングの転送
	lightObj_->Draw();
	//シャドウの転送
	shadow_->Draw();

	///背景スプライトの描画開始 

	

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始

	//sprite_->Draw(spriteInfo_);

	///前面スプライトの描画終了

	///オブジェクトの描画開始

	//yukariModel_->Draw(yukariModelInfo_);
	//groundModel_->Draw(groundModelInfo_, uvCheckerHandle_);
	//cubeModel_->Draw(cubeModelInfo_);
	testModel_->Draw(testModelInfo_);

	///オブジェクトの描画終了

	///パーティクルの描画


	///パーティクルの描画終了
}