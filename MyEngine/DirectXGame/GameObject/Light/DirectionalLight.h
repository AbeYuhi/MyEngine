#pragma once
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector3_Math.hpp"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

struct DirectionalLightData {
	int32_t type;
	float padding[3];
	Vector4 color;
	Vector3 direction;
	float intensity;
};

class DirectionalLight {
public: //静的メンバ関数
	static std::unique_ptr<DirectionalLight> Create();
	static int sLightNum_;

public: //メンバ関数
	DirectionalLight();
	~DirectionalLight();

	void Initialize();

	void Update();

	void Draw();

public: //ゲッターセッター

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;

	DirectionalLightData* directionalLightData_;
	int lightNum_;
};