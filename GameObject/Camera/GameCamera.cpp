#include "GameCamera.h"

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::Initialize() {

	transform_.Initialize();
	transform_.translate_.y = 5;
	transform_.translate_.z = -10;
	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void GameCamera::Update() {
	InputManager* input = InputManager::GetInstance();

	ImGui::Begin("CameraManager");
	ImGui::SliderFloat3("CameraTranslate", &transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("CameraRotate", &transform_.rotate_.x, -2.0f * M_PI, 2.0f * M_PI);
	ImGui::End();

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
		transform_.rotate_.y += input->GetGamePadRStick().x * 0.05f;
		transform_.rotate_.x += input->GetGamePadRStick().y * -0.05f;

		velocity.x = input->GetGamePadLStick().x * 0.5f;
		velocity.z = input->GetGamePadLStick().y * 0.5f;
	}

	
	Vector3 rotate = { 0, transform_.rotate_.y, 0 };
	Matrix4x4 matrix = MakeAffineMatrix(transform_.scale_, rotate, transform_.translate_);

	velocity = TransformNormal(velocity, matrix);
	transform_.translate_ += velocity;

	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, (float)WinApp::kWindowWidth / (float)WinApp::kWindowHeight, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}