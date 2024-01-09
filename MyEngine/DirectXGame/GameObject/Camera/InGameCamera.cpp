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

	Vector3 velocity = { 0 };

	transform_.rotate_.y += input->GetMouseMovement().x * 0.005f;
	transform_.rotate_.x += input->GetMouseMovement().y * 0.005f;

	if (input->IsPushKey(DIK_W)) {
		velocity.z += 0.5f;
	}
	if (input->IsPushKey(DIK_S)) {
		velocity.z -= 0.5f;
	}
	if (input->IsPushKey(DIK_A)) {
		velocity.x -= 0.5f;
	}
	if (input->IsPushKey(DIK_D)) {
		velocity.x += 0.5f;
	}

	if (input->IsControllerConnected()) {
		transform_.rotate_.y += input->GetGamePadRStick().x * 0.005f;
		transform_.rotate_.x += input->GetGamePadRStick().y * -0.005f;

		velocity.x = input->GetGamePadLStick().x * 0.5f;
		velocity.z = input->GetGamePadLStick().y * 0.5f;
	}

	
	Vector3 rotate = { 0, transform_.rotate_.y, 0 };
	Matrix4x4 matrix = MakeAffineMatrix(transform_.scale_, rotate, transform_.translate_);

	velocity = TransformNormal(velocity, matrix);
	transform_.translate_ += velocity;

	Base3DCamera::Update();
}