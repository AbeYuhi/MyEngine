#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}

int DirectionalLight::sLightNum_ = 0;

std::unique_ptr<DirectionalLight> DirectionalLight::Create() {
	sLightNum_++;
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
	lightNum_ = sLightNum_;
}

void DirectionalLight::Update() {

	std::string number = "DirectionalLight" + std::to_string(lightNum_);
	ImGui::Begin(number.c_str());
	const char* modes[] = { "NONE", "LAMBERT", "HALFLAMBERT" };
	ImGui::Combo("LightingMode", &directionalLightData_->type, modes, IM_ARRAYSIZE(modes));
	ImGui::ColorEdit3("color", &directionalLightData_->color.x);
	ImGui::SliderFloat3("direction", &directionalLightData_->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("intensity", &directionalLightData_->intensity, 0.0f, 1.0f);
	ImGui::End();

	directionalLightData_->direction = Normalize(directionalLightData_->direction);
	if (std::isnan(directionalLightData_->direction.x)) {
		directionalLightData_->direction = { 0.0f, -1.0f, 0.0f };
	}
}

void DirectionalLight::Draw() {
	//パイプラインステートの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(GraphicsPipelineManager::GetInstance()->GetPSO(kDefault));
	//ルートシグネチャの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(GraphicsPipelineManager::GetInstance()->GetRootSignature(kDefault));
	//ライトの設定をコマンドリストに積む
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, directionalLightResource_->GetGPUVirtualAddress());
}