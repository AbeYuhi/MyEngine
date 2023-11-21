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

struct PointLightData {
	int32_t type;
	float padding[3];
	Vector4 color;
	Vector3 position;
	float intensity;
	float radius;
	float decay;
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