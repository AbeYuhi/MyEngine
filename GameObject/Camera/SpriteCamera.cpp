#include "SpriteCamera.h"

SpriteCamera::SpriteCamera()
{
}

SpriteCamera::~SpriteCamera()
{
}

void SpriteCamera::Initialize() {

	transform_.scale = { 1, 1, 1 };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, 0.0f };
	worldMatrix_ = MakeAffineMatrix(transform_);

	//マウスの位置
	mousePos_ = { 0, 0 };
	preMousePos_ = { 0, 0 };
	isMove_ = false;

	//カメラが切り取る範囲
	nearClip_ = 0.0f;
	farClip_ = 100.0f;
}

void SpriteCamera::Update() {
	WinApp* winApp = WinApp::GetInstance();

	ImGui::Begin("SpriteCameraManager");
	ImGui::Checkbox("IsMove", &isMove_);
	ImGui::SliderFloat3("CameraTranslate", &transform_.translate.x, -100.0f, 100.0f);
	ImGui::End();

	if (isMove_) {
		Vector3 prePos = transform_.translate;

		preMousePos_ = mousePos_;
		mousePos_ = ImGui::GetMousePos();

		Vector3 cameraVelocity = { 0.0f, 0.0f, 0.0f };
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			cameraVelocity.x += -2;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			cameraVelocity.x += 2;
		}
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			cameraVelocity.y += 2;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			cameraVelocity.y += -2;
		}

		if (ImGui::IsMouseDown(2)) {
			ImVec2 mouseAmount;
			mouseAmount.x = mousePos_.x - preMousePos_.x;
			mouseAmount.y = mousePos_.y - preMousePos_.y;

			cameraVelocity.x += -mouseAmount.x * 0.05f;
			cameraVelocity.y += mouseAmount.y * 0.05f;
		}

		transform_.translate += TransformNormal(cameraVelocity, worldMatrix_);
	}

	worldMatrix_ = MakeAffineMatrix(transform_);

	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakeOrthographicMatrix(0.0f, 0.0f, (float)winApp->kWindowWidth, (float)winApp->kWindowHeight, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}