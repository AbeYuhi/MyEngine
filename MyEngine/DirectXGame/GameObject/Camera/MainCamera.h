#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector3_Math.hpp"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Data/CameraForGPU.h"

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