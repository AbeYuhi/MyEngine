#include "SpriteCamera.h"

SpriteCamera* SpriteCamera::GetInstance() {
	static SpriteCamera instance;
	return &instance;
}

void SpriteCamera::Initialize() {

	transform_.scale_ = { 1, 1, 1 };
	transform_.rotate_ = { 0.0f, 0.0f, 0.0f };
	transform_.translate_ = { 0.0f, 0.0f, 0.0f };
	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);

	//マウスの位置
	isMove_ = false;

	//カメラが切り取る範囲
	nearClip_ = 0.0f;
	farClip_ = 100.0f;
}

void SpriteCamera::Update() {
	WinApp* winApp = WinApp::GetInstance();
	
#ifdef _DEBUG
	ImGui::Begin("SpriteCameraManager");
	ImGui::Checkbox("IsMove", &isMove_);
	ImGui::SliderFloat3("CameraTranslate", &transform_.translate_.x, -100.0f, 100.0f);
	ImGui::End();

	if (isMove_) {
		InputManager* input = InputManager::GetInstance();

		Vector3 cameraVelocity = { 0.0f, 0.0f, 0.0f };
		if (input->IsPushKey(DIK_A)) {
			cameraVelocity.x += -2;
		}
		if (input->IsPushKey(DIK_D)) {
			cameraVelocity.x += 2;
		}
		if (input->IsPushKey(DIK_W)) {
			cameraVelocity.y += -2;
		}
		if (input->IsPushKey(DIK_S)) {
			cameraVelocity.y += 2;
		}

		if (input->IsMousePush(2)) {
			ImVec2 mouseAmount;
			mouseAmount.x = input->GetMouseMovement().x;
			mouseAmount.y = input->GetMouseMovement().y;

			cameraVelocity.x += -mouseAmount.x * 0.05f;
			cameraVelocity.y += mouseAmount.y * 0.05f;
		}

		transform_.translate_ += TransformNormal(cameraVelocity, worldMatrix_);
	}

#endif // _DEBUG

	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakeOrthographicMatrix(0.0f, 0.0f, (float)winApp->kWindowWidth, (float)winApp->kWindowHeight, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}