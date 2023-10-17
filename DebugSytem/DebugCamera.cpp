#include "DebugCamera.h"

DebugCamera::DebugCamera()
{
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Initialize() {

	transform_.scale = { 1, 1, 1 };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, -10.0f };
	worldMatrix_ = MakeAffineMatrix(transform_);

	//マウスの位置
	mousePos_ = { 0, 0 };
	preMousePos_ = { 0, 0 };

	//
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void DebugCamera::Update() {

	/*ImGui::Begin("CameraManager");
	ImGui::SliderFloat3("CameraTranslate", &translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("CameraRotate", &rotate_.x, -2.0f * M_PI, 2.0f * M_PI);
	ImGui::End();

	Vector3 prePos = translate_;
	preMousePos_ = mousePos_;
	Novice::GetMousePosition(&mousePos_.x, &mousePos_.y);

	if (Novice::IsPressMouse(1)) {
		Vector2Int mouseAmount = mousePos_ - preMousePos_;

		rotate_.x += mouseAmount.y * 0.01f;
		rotate_.y += mouseAmount.x * 0.01f;
	}

	Vector3 cameraVelocity = { 0.0f, 0.0f, 0.0f };
	if (keys[DIK_A]) {
		cameraVelocity.x += -0.2f;
	}
	if (keys[DIK_D]) {
		cameraVelocity.x += 0.2f;
	}
	if (keys[DIK_W]) {
		cameraVelocity.z += 0.2f;
	}
	if (keys[DIK_S]) {
		cameraVelocity.z += -0.2f;
	}

	if (Novice::IsPressMouse(2)) {
		Vector2Int mouseAmount = mousePos_ - preMousePos_;

		cameraVelocity.x += -mouseAmount.x * 0.1f;
		cameraVelocity.y += mouseAmount.y * 0.1f;
	}

	int wheel = Novice::GetWheel();
	if (wheel != 0) {
		cameraVelocity.z += wheel * 0.01f;
	}

	translate_ += TransformNormal(cameraVelocity, worldMatrix_);

	if (keys[DIK_LSHIFT]) {
		translate_.y = prePos.y;
	}

	worldMatrix_ = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate_, translate_);*/

	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, (float)WinApp::kWindowWidth / (float)WinApp::kWindowHeight, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}