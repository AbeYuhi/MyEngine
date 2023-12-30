#include "GameClearCamera.h"

GameClearCamera::GameClearCamera() {}
GameClearCamera::~GameClearCamera() {}

void GameClearCamera::Initialize() {

	//カメラの初期化
	Base3DCamera::Initialize();

	cameraName_ = "GameClear";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void GameClearCamera::Update() {



	Base3DCamera::Update();
}
