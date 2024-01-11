#include "InGameCamera.h"

InGameCamera::InGameCamera()
{
}

InGameCamera::~InGameCamera()
{
}

void InGameCamera::Initialize() {

	//カメラの初期化
	Base3DCamera::Initialize();

	cameraName_ = "inGame";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void InGameCamera::Update() {
	InputManager* input = InputManager::GetInstance();



	Base3DCamera::Update();
}