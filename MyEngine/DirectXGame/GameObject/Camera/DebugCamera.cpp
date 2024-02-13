#include "DebugCamera.h"

DebugCamera::DebugCamera()
{
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Initialize() {

	transform_.scale_ = { 1, 1, 1 };
	transform_.rotate_ = { 0.0f, 0.0f, 0.0f };
	transform_.translate_ = { 0.0f, 0.0f, -10.0f };
	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
	isMove_ = false;
}

void DebugCamera::Update() {
	InputManager* input = InputManager::GetInstance();

#ifdef _DEBUG
	ImGui::Begin("CameraManager");
	ImGui::Checkbox("IsMoveCamera", &isMove_);
	ImGui::SliderFloat3("CameraTranslate", &transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("CameraRotate", &transform_.rotate_.x, -2.0f * M_PI, 2.0f * M_PI);
	ImGui::End();
#endif // _DEBUG

	if (isMove_) {
		if (input->IsMousePush(0)) {

			transform_.rotate_.x += input->GetMouseMovement().y * 0.005f;
			transform_.rotate_.y += input->GetMouseMovement().x * 0.005f;
		}

		Vector3 cameraVelocity = { 0.0f, 0.0f, 0.0f };
		if (input->IsPushKey(DIK_A)) {
			cameraVelocity.x += -0.2f;
		}
		if (input->IsPushKey(DIK_D)) {
			cameraVelocity.x += 0.2f;
		}
		if (input->IsPushKey(DIK_W)) {
			cameraVelocity.z += 0.2f;
		}
		if (input->IsPushKey(DIK_S)) {
			cameraVelocity.z += -0.2f;
		}

		if (input->IsMousePush(2)) {
			cameraVelocity.x -= input->GetMouseMovement().x * 0.01f;
			cameraVelocity.y += input->GetMouseMovement().y * 0.01f;
		}

		float wheel = input->GetMouseWheelDelta();
		if (wheel != 0) {
			cameraVelocity.z += wheel * 0.02f;
		}

		if (input->IsPushKey(DIK_LSHIFT)) {
			cameraVelocity.y = 0;
		}

		transform_.translate_ += TransformNormal(cameraVelocity, worldMatrix_);
	}

	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);

	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, (float)WinApp::kWindowWidth / (float)WinApp::kWindowHeight, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}