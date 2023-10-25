#pragma once
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector3_Math.hpp"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../../Base/WinApp/WinApp.h"
#include "../../Base/DirectXCommon/DirectXCommon.h"
#include "../../Manager/TextureManager.h"
#include "../../Manager/ImGuiManager.h"
#include "../../Manager/GraphicsPipelineManager.h"

struct DirectionalLightData {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

class DirectionalLight {
public: //静的メンバ関数
	static std::unique_ptr<DirectionalLight> Create();

public: //メンバ関数
	DirectionalLight();
	~DirectionalLight();

	void Initialize();

	void Update();

public: //ゲッターセッター

private: //メンバ変数
	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;

	DirectionalLightData* directionalLightData_;
};