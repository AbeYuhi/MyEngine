#include "GameOverCamera.h"

GameOverCamera::GameOverCamera() {}
GameOverCamera::~GameOverCamera() {}

void GameOverCamera::Initialize() {

	//カメラの初期化
	Base3DCamera::Initialize();

	cameraName_ = "GameOver";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void GameOverCamera::Update() {



	Base3DCamera::Update();
}
