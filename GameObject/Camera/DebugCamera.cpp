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

	//マウスの位置
	mousePos_ = { 0, 0 };
	preMousePos_ = { 0, 0 };

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
	isMove_ = false;
}

void DebugCamera::Update() {
	ImGui::Begin("CameraManager");
	ImGui::Checkbox("IsMoveCamera", &isMove_);
	ImGui::SliderFloat3("CameraTranslate", &transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("CameraRotate", &transform_.rotate_.x, -2.0f * M_PI, 2.0f * M_PI);
	ImGui::End();

	Vector3 prePos = transform_.translate_;
	preMousePos_ = mousePos_;
	mousePos_ = ImGui::GetMousePos();

	if (isMove_) {
		if (ImGui::IsMouseDown(0)) {
			ImVec2 mouseAmount;
			mouseAmount.x = mousePos_.x - preMousePos_.x;
			mouseAmount.y = mousePos_.y - preMousePos_.y;

			transform_.rotate_.x += mouseAmount.y * 0.005f;
			transform_.rotate_.y += mouseAmount.x * 0.005f;
		}

		Vector3 cameraVelocity = { 0.0f, 0.0f, 0.0f };
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			cameraVelocity.x += -0.2f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			cameraVelocity.x += 0.2f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			cameraVelocity.z += 0.2f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			cameraVelocity.z += -0.2f;
		}

		if (ImGui::IsMouseDown(2)) {
			ImVec2 mouseAmount;
			mouseAmount.x = mousePos_.x - preMousePos_.x;
			mouseAmount.y = mousePos_.y - preMousePos_.y;

			cameraVelocity.x += -mouseAmount.x * 0.05f;
			cameraVelocity.y += mouseAmount.y * 0.05f;
		}

		float wheel = ImGui::GetIO().MouseWheel;
		if (wheel != 0) {
			cameraVelocity.z += wheel * 0.2f;
		}

		transform_.translate_ += TransformNormal(cameraVelocity, worldMatrix_);

		if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
			transform_.translate_.y = prePos.y;
		}
	}

	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);

	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, (float)WinApp::kWindowWidth / (float)WinApp::kWindowHeight, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}