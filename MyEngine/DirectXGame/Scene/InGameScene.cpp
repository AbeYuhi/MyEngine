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

	//ブレンドモード
	blendMode_ = kBlendModeNormal;

	//画像読み込み
	monsterBallHandle_ = TextureManager::Load("monsterBall.png");
	fenceHandle_ = TextureManager::Load("fence.png");

	//ゲームオブジェクト
	testParticle1_ = std::make_unique<TestParticle>(100);
	testParticle1_->Initialize();

	spriteParticle_ = std::make_unique<SpriteParticle>(100);
	spriteParticle_->Initialize();

	planeParticle_ = std::make_unique<PlaneParticle>(100);
	planeParticle_->Initialize();

	groundModel_ = Model::Create("yukari", "yukari.obj");
	groundModelInfo_.Initialize();
	groundModelInfo_.materialInfo_.material_->enableLightint = false;

	sphereModel_ = Model::Create("sphere", "sphere.obj");
	sphereModelInfo_.Initialize();
	sphereModelInfo_.materialInfo_.material_->enableLightint = true;

	planeOBJModel_ = Model::Create("plane", "plane.obj");
	planeOBJModelInfo_.Initialize();
	planeOBJModelInfo_.materialInfo_.material_->enableLightint = false;

	planeGLTFModel_ = Model::Create("Material01", "Material_01.gltf");
	planeGLTFModelInfo_.Initialize();
	planeGLTFModelInfo_.materialInfo_.material_->enableLightint = false;

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
	//testParticle1_->Update();
	//spriteParticle_->Update();
	//planeParticle_->Update();

	ImGui::BeginTabBar("RenderItemInfo");
	if (ImGui::BeginTabItem("groundModel")) {
		ImGui::SliderFloat3("pos", &groundModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &groundModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &groundModelInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &groundModelInfo_.materialInfo_.material_->shininess, 0, 100);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("sphereModel")) {
		ImGui::SliderFloat3("pos", &sphereModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &sphereModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &sphereModelInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &sphereModelInfo_.materialInfo_.material_->shininess, 0, 100);
		bool b = 0;
		if (sphereModelInfo_.materialInfo_.material_->isSpecularReflection == 0) {
			b = false;
		}
		else {
			b = true;
		}
		ImGui::Checkbox("isSpecularReflection", &b);
		sphereModelInfo_.materialInfo_.material_->isSpecularReflection = b;
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("planeOBJModel")) {
		ImGui::SliderFloat3("pos", &planeOBJModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &planeOBJModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &planeOBJModelInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &planeOBJModelInfo_.materialInfo_.material_->shininess, 0, 100);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("planeGLTFModel")) {
		ImGui::SliderFloat3("pos", &planeGLTFModelInfo_.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &planeGLTFModelInfo_.worldTransform_.data_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("scale", &planeGLTFModelInfo_.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &planeGLTFModelInfo_.materialInfo_.material_->shininess, 0, 100);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen"};
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();

	groundModelInfo_.Update();
	sphereModelInfo_.Update();
	planeOBJModelInfo_.Update();
	planeGLTFModelInfo_.Update();
}

void InGameScene::Draw() {
	//カメラの転送
	mainCamera_->Draw();
	//ライティングの転送
	lightObj_->Draw();

	///背景スプライトの描画開始 

	

	///背景スプライト描画終了
	//深度バッファのクリア
	directXCommon_->ClearDepthStencilBuffer();

	///前面スプライトの描画開始



	//spriteParticle_->EmitterDraw();

	///前面スプライトの描画終了

	///オブジェクトの描画開始

	//planeParticle_->EmitterDraw();
	planeOBJModel_->Draw(planeOBJModelInfo_);
	planeGLTFModel_->Draw(planeGLTFModelInfo_);
	sphereModel_->Draw(sphereModelInfo_, monsterBallHandle_);
	groundModel_->Draw(groundModelInfo_);

	///オブジェクトの描画終了

	///パーティクルの描画

	//spriteParticle_->Draw();
	//planeParticle_->Draw();

	///パーティクルの描画終了
}