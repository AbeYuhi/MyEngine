#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}

std::unique_ptr<DirectionalLight> DirectionalLight::Create() {
	std::unique_ptr<DirectionalLight> object = std::make_unique<DirectionalLight>();
	object->Initialize();

	return object;
}

void DirectionalLight::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//transformationMatrixResourceの生成
	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLightData));

	//MaterialDataデータの記入
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//色の書き込み
	directionalLightData_->type = 2;
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_->intensity = 1.0f;
}

void DirectionalLight::Update() {
	ImGui::Begin("DirectionalLight");
	const char* modes[] = {"NONE", "LAMBERT", "HALFLAMBERT"};
	ImGui::Combo("LightingMode", &directionalLightData_->type, modes, IM_ARRAYSIZE(modes));
	ImGui::ColorEdit3("color", &directionalLightData_->color.x);
	ImGui::SliderFloat3("direction", &directionalLightData_->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("intensity", &directionalLightData_->intensity, 0.0f, 1.0f);
	ImGui::End();
	directionalLightData_->direction = Normalize(directionalLightData_->direction);
	if (std::isnan(directionalLightData_->direction.x)) {
		directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	}

	//パイプラインステートの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(GraphicsPipelineManager::GetInstance()->GetPSO());
	//ルートシグネチャの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(GraphicsPipelineManager::GetInstance()->GetRootSignature());
	//ライトの設定をコマンドリストに積む
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
}