#include "GameObject/Light/PointLight.h"


//コンストラクタ
PointLight::PointLight() {}
PointLight::~PointLight() {}

//静的変数の初期化
int PointLight::sLightNum_ = 0;
std::unique_ptr<PointLight> PointLight::Create() {
	sLightNum_++;
	std::unique_ptr<PointLight> object = std::make_unique<PointLight>();
	object->Initialize();

	return object;
}

void PointLight::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//transformationMatrixResourceの生成
	pointLightResource_ = CreateBufferResource(sizeof(PointLightData));

	//MaterialDataデータの記入
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	//色の書き込み
	pointLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLightData_->position = { 0.0f, 0.0f, 0.0f };
	pointLightData_->intensity = 1.0f;
	lightNum_ = sLightNum_;
}

void PointLight::Update() {

	std::string number = "PointLight" + std::to_string(lightNum_);
	ImGui::Begin(number.c_str());
	ImGui::ColorEdit3("color", &pointLightData_->color.x);
	ImGui::SliderFloat3("position", &pointLightData_->position.x, -10, 10);
	ImGui::SliderFloat("intensity", &pointLightData_->intensity, 0.0f, 1.0f);
	ImGui::End();

	//パイプラインステートの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(GraphicsPipelineManager::GetInstance()->GetPSO());
	//ルートシグネチャの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(GraphicsPipelineManager::GetInstance()->GetRootSignature());
	//ライトの設定をコマンドリストに積む
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, pointLightResource_->GetGPUVirtualAddress());
}