#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector3_Math.hpp"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/TextureManager.h"
#include "Manager/ImGuiManager.h"
#include "Manager/GraphicsPipelineManager.h"

struct SpotLightData {
	Vector3 position;
	Vector3 rotate;
};

class SpotLight {
public: //静的メンバ関数
	static std::unique_ptr<SpotLight> Create();
	static int sLightNum_;

public: //メンバ関数
	SpotLight();
	~SpotLight();

	void Initialize();

	void Update();

public: //ゲッターセッター

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;

	SpotLightData* directionalLightData_;
	int lightNum_;
};