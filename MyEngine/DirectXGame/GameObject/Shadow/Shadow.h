#pragma once
#include <directXMath.h>
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/Math.h"

class Shadow
{
public:

	void Initialize();

	void Update(Vector3 lightVec);

	void Draw();

private:
	ComPtr<ID3D12Resource> shadowResorce_ = nullptr;
	Matrix4x4* shadowData_;
};

