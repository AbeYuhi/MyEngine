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

	//画面に表示する際のVPmatrix
	viewProjectionMatrix_ = MakeIdentity4x4();
	//インゲームカメラ
	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->Initialize();
	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
#ifdef _DEBUG
	isDebugCamera_ = true;
#else
	isDebugCamera_ = false;
#endif // _DEBUG
	//スプライト専用カメラ
	spriteCamera_ = std::make_unique<SpriteCamera>();
	spriteCamera_->Initialize();

	//平行光源
	directionalLight_ = DirectionalLight::Create();

	/*Vector3 triangle2Pos[3] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f} };
	triangle2_ = Triangle::Create(triangle2Pos);*/

	//スプライトの生成
	sprite_ = Sprite::Create({620, 360});
	spriteInfo_.Initialize(spriteCamera_->GetViewProjectionMatrixPointer());

	//モデルの生成
	model_ = Model::Create("fence");
	modelRenderInfo_.Initialize(&viewProjectionMatrix_);
	modelRenderInfo_.worldTransform_.data_.translate_.z = 10;

	blendMode_ = kBlendModeNone;
}

void GameScene::Update() {
	//カメラの更新
#ifdef _DEBUG
	ImGui::Begin("Debug");
	ImGui::Checkbox("UseDebugCamera", &isDebugCamera_);
	ImGui::End();
#endif // _DEBUG
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

	ImGui::Begin("modelRenderInfo");
	ImGui::SliderFloat3("translate", &modelRenderInfo_.worldTransform_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &modelRenderInfo_.worldTransform_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &modelRenderInfo_.worldTransform_.data_.scale_.x, -10, 10);
	ImGui::DragFloat2("UVTransform", &modelRenderInfo_.materialInfo_.uvTransform_.translate_.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &modelRenderInfo_.materialInfo_.uvTransform_.scale_.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &modelRenderInfo_.materialInfo_.uvTransform_.rotate_.z);
	ImGui::ColorEdit4("Color", &modelRenderInfo_.materialInfo_.material_->color.x);
	if (ImGui::Button("IsLighting")) {
		bool isLighting = modelRenderInfo_.materialInfo_.material_->enableLightint;
		modelRenderInfo_.materialInfo_.material_->enableLightint = !isLighting;
	}
	ImGui::End();

	ImGui::Begin("spriteInfo");
	ImGui::SliderFloat3("translate", &spriteInfo_.worldTransform_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &spriteInfo_.worldTransform_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &spriteInfo_.worldTransform_.data_.scale_.x, -10, 10);
	ImGui::DragFloat2("UVTransform", &spriteInfo_.materialInfo_.uvTransform_.translate_.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &spriteInfo_.materialInfo_.uvTransform_.scale_.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &spriteInfo_.materialInfo_.uvTransform_.rotate_.z);
	ImGui::ColorEdit4("Color", &spriteInfo_.materialInfo_.material_->color.x);
	ImGui::End();

	ImGui::Begin("BlendMode");
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen"};
	ImGui::Combo("blendMode", &blendMode_, modes, IM_ARRAYSIZE(modes));
	GraphicsPipelineManager::GetInstance()->SetBlendMode(static_cast<BlendMode>(blendMode_));
	ImGui::End();

	spriteInfo_.Update();
	modelRenderInfo_.Update();
}

void GameScene::Draw() {

	//sprite_->Draw(spriteInfo_, FENCE);
	model_->Draw(modelRenderInfo_);

}