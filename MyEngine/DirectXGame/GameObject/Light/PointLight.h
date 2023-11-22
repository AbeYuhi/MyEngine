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

struct PointLightData {
	int32_t type;
	float padding[3];
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
	float padding2[2];
};

class PointLight {
public: //静的メンバ関数
	static std::unique_ptr<PointLight> Create();
	static int sLightNum_;

public: //メンバ関数
	PointLight();
	~PointLight();

	void Initialize();

	void Update();

	void Draw();

public: //ゲッターセッター

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> pointLightResource_ = nullptr;

	PointLightData* pointLightData_;
	int lightNum_;
};