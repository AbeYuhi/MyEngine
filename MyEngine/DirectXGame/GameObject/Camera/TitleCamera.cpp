#include "TitleCamera.h"

TitleCamera::TitleCamera(){}
TitleCamera::~TitleCamera(){}

void TitleCamera::Initialize() {
	
	//カメラの初期化
	Base3DCamera::Initialize();

	cameraName_ = "title";

	//カメラが切り取る範囲
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	fovY_ = 0.45f;
}

void TitleCamera::Update() {



	Base3DCamera::Update();
}
