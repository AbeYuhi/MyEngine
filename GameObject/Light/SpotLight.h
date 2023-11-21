#pragma once
#include <numbers>
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

struct SpotLightData
{
	int32_t lightingType;
	float padding[3];
	Vector4 color;
	Vector3 position;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
	float falloffStart;
	float padding2;
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

	void Draw();

public: //ゲッターセッター

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> spotLightResource_ = nullptr;

	SpotLightData* spotLightData_;
	int lightNum_;
};