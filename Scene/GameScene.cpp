#include "GameScene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
}

void GameScene::Initialize() {
	//基本機能の初期化
	winApp_ = WinApp::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	spriteCamera_ = std::make_unique<SpriteCamera>();
	spriteCamera_->Initialize();

	//スフィア
	sphere_ = Sphere::Create();
	sphereTexture_ = UVCHECKER;
	isUseMonsterBall_ = false;

	/*Vector3 triangle2Pos[3] = { {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, -0.5f, -0.5f} };
	triangle2_ = Triangle::Create(triangle2Pos);*/

	sprite_ = Sprite::Create({620, 360});
}

void GameScene::Update() {
	//カメラの更新
	debugCamera_->Update();
	spriteCamera_->Update();

	ImGui::Begin("Texture");
	ImGui::Checkbox("useMonsterBall", &isUseMonsterBall_);
	ImGui::End();
	if (isUseMonsterBall_) {
		sphereTexture_ = MONSTERBALL;
	}
	else {
		sphereTexture_ = UVCHECKER;
	}
	sphere_->Update();
	sphere_->SetRotate({ sphere_->GetRotate().x, sphere_->GetRotate().y + 0.03f, sphere_->GetRotate().z });

	sprite_->Update();
}

void GameScene::Draw() {

	//三角形
	Triangle::PreDraw(directXCommon_->GetCommandList());

	Triangle::PostDraw();

	//球
	Sphere::PreDraw(directXCommon_->GetCommandList());

	sphere_->Draw(debugCamera_->GetViewProjectionMatrix(), sphereTexture_);

	Sphere::PostDraw();

	//スプライト
	Sprite::PreDraw(directXCommon_->GetCommandList());

	sprite_->Draw(spriteCamera_->GetViewProjectionMatrix());

	Sprite::PostDraw();

}