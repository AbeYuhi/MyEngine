#include "MenuCamera.h"

MenuCamera::MenuCamera() {}
MenuCamera::~MenuCamera() {}

void MenuCamera::Initialize() {

	//カメラの初期化
	Base3DCamera::Initialize();

	cameraName_ = "Menu";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void MenuCamera::Update() {



	Base3DCamera::Update();
}
