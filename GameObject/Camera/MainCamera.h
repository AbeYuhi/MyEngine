#pragma once
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/CreateResource/CreateResource.h"
#include "Manager/GraphicsPipelineManager.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector3_Math.hpp"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Data/CameraForGPU.h"

class MainCamera {
public:
	MainCamera() = default;
	~MainCamera() = default;

	void Initialize();

	void Update(Matrix4x4 viewProjectionMatrix, Vector3 cameraWorldPos);

	void Draw();

	Matrix4x4* GetPViewProjectionMatrix() { return &viewProjectionMatrix_; }

private:
	ComPtr<ID3D12Resource> cameraResorce_ = nullptr;
	CameraForGPU* cameraData_;
	Matrix4x4 viewProjectionMatrix_;
};