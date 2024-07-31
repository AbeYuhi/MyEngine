#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/CameraForGPU.h"
#include "DirectXGame/Data/Transform.h"

class MainCamera {
public:
	static MainCamera* GetInstance();

	void Initialize();

	void Update(EulerTransformData transform_, Matrix4x4 worldMatrix = MakeIdentity4x4(), Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, (float)WinApp::kWindowWidth / (float)WinApp::kWindowHeight, 0.1f, 100.0f));

	void Draw();

	Matrix4x4 GetWorldMatrix() { return worldMatrix_; }
	Matrix4x4 GetViewProjectionMatrix() { return viewProjectionMatrix_; }
	Matrix4x4* GetPViewProjectionMatrix() { return &viewProjectionMatrix_; }
	Vector3 GetWorldPos() { return { worldMatrix_.m[3][0], worldMatrix_.m[3][1] , worldMatrix_.m[3][2] }; }

private:
	MainCamera() = default;
	~MainCamera() = default;
	ComPtr<ID3D12Resource> cameraResorce_ = nullptr;
	CameraForGPU* cameraData_;
	EulerTransformData transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
};