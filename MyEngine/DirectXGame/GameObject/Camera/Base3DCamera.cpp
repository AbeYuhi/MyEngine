#include "Base3DCamera.h"

Base3DCamera::Base3DCamera()
{
}

Base3DCamera::~Base3DCamera()
{
}

void Base3DCamera::Initialize() {

	transform_.Initialize();
	transform_.translate_.z = -10;
	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void Base3DCamera::Update() {
	InputManager* input = InputManager::GetInstance();

#ifdef _DEBUG
	std::string cameraObjName = "Camera" + cameraName_;
	ImGui::Begin(cameraObjName.c_str());
	ImGui::SliderFloat3("CameraTranslate", &transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("CameraRotate", &transform_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::End();
#endif // _DEBUG

	worldMatrix_ = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, (float)WinApp::kWindowWidth / (float)WinApp::kWindowHeight, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}