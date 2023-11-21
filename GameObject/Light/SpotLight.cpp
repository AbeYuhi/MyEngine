#include "SpotLight.h"

//コンストラクタ
SpotLight::SpotLight() {}
SpotLight::~SpotLight() {}

//静的変数の初期化
int SpotLight::sLightNum_ = 0;
std::unique_ptr<SpotLight> SpotLight::Create() {
	sLightNum_++;
	std::unique_ptr<SpotLight> object = std::make_unique<SpotLight>();
	object->Initialize();

	return object;
}

void SpotLight::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//transformationMatrixResourceの生成
	spotLightResource_ = CreateBufferResource(sizeof(SpotLightData));

	//MaterialDataデータの記入
	spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	//色の書き込み
	spotLightData_->lightingType = 2;
	spotLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	spotLightData_->position = { 0, 2, -5 };
	spotLightData_->intensity = 5.0f;
	spotLightData_->direction = { 0, 0, 1.0f };
	spotLightData_->distance = 10.0f;
	spotLightData_->decay = 5.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->falloffStart = 1.0f;
	lightNum_ = sLightNum_;
}

void SpotLight::Update() {
	std::string number = "SpotLight" + std::to_string(lightNum_);
	ImGui::Begin(number.c_str());
	const char* modes[] = { "NONE", "LAMBERT", "HALFLAMBERT" };
	ImGui::Combo("LightingMode", &spotLightData_->lightingType, modes, IM_ARRAYSIZE(modes));
	ImGui::ColorEdit3("color", &spotLightData_->color.x);
	ImGui::SliderFloat3("position", &spotLightData_->position.x, -10, 10);
	ImGui::SliderFloat("intensity", &spotLightData_->intensity, 0.0f, 50.0f);
	ImGui::SliderFloat3("direction", &spotLightData_->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("distance", &spotLightData_->distance, 0, 100);
	ImGui::SliderFloat("decay", &spotLightData_->decay, 0, 10);
	ImGui::SliderFloat("cosAngle", &spotLightData_->cosAngle, 0, 1);
	ImGui::SliderFloat("falloffStart", &spotLightData_->falloffStart, spotLightData_->cosAngle + 0.01f, 1);
	ImGui::End();

	spotLightData_->direction = Normalize(spotLightData_->direction);
	if (std::isnan(spotLightData_->direction.x)) {
		spotLightData_->direction = { 0.0f, -1.0f, 0.0f };
	}
}

void SpotLight::Draw() {
	//パイプラインステートの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(GraphicsPipelineManager::GetInstance()->GetPSO());
	//ルートシグネチャの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(GraphicsPipelineManager::GetInstance()->GetRootSignature());
	//ライトの設定をコマンドリストに積む
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLightResource_->GetGPUVirtualAddress());

}