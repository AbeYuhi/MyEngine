#include "LightObject.h"

void LightObject::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//transformationMatrixResourceの生成
	lightResource_ = CreateBufferResource(sizeof(LightData));

	//情報の格納場所
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));

	for (int index = 0; index < kDirectionLightNum; index++) {
		lightData_->directionalLight[index].type = 2;
		lightData_->directionalLight[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		lightData_->directionalLight[index].direction = { 0.0f, -1.0f, 0.0f };
		lightData_->directionalLight[index].intensity = 1.0f;
	}

	for (int index = 0; index < kPointLightNum; index++) {
		lightData_->pointLight[index].type = 0;
		lightData_->pointLight[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		lightData_->pointLight[index].position = { 0.0f, 0.0f, 0.0f };
		lightData_->pointLight[index].intensity = 1.0f;
		lightData_->pointLight[index].radius = 100;
		lightData_->pointLight[index].decay = 0.0f;
	}

	for (int index = 0; index < kSpotLightNum; index++) {
		lightData_->spotLight[index].lightingType = 0;
		lightData_->spotLight[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		lightData_->spotLight[index].position = { 0, 2, -5 };
		lightData_->spotLight[index].intensity = 5.0f;
		lightData_->spotLight[index].direction = { 0, 0, 1.0f };
		lightData_->spotLight[index].distance = 10.0f;
		lightData_->spotLight[index].decay = 5.0f;
		lightData_->spotLight[index].cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
		lightData_->spotLight[index].falloffStart = 1.0f;
	}
}

void LightObject::Update() {

#ifdef _DEBUG

	ImGui::Begin("DirectionLight");
	ImGui::BeginTabBar("DirectionLight");
	for (int index = 0; index < kDirectionLightNum; index++) {
		std::string tabItemName = std::to_string(index);
		if (ImGui::BeginTabItem(tabItemName.c_str())) {
			const char* modes[] = { "NONE", "LAMBERT", "HALFLAMBERT" };
			ImGui::Combo("LightingMode", &lightData_->directionalLight[index].type, modes, IM_ARRAYSIZE(modes));
			ImGui::ColorEdit3("color", &lightData_->directionalLight[index].color.x);
			ImGui::SliderFloat3("direction", &lightData_->directionalLight[index].direction.x, -1.0f, 1.0f);
			ImGui::SliderFloat("intensity", &lightData_->directionalLight[index].intensity, 0.0f, 1.0f);
			ImGui::EndTabItem();
		}
		/*lightData_->directionalLight[index].direction = Normalize(lightData_->directionalLight[index].direction);
		if (std::isnan(lightData_->directionalLight[index].direction.x)) {
			lightData_->directionalLight[index].direction = { 0.0f, -1.0f, 0.0f };
		}*/
	}
	ImGui::EndTabBar();
	ImGui::End();

	ImGui::Begin("PointLight");
	ImGui::BeginTabBar("PointLight");
	for (int index = 0; index < kPointLightNum; index++) {
		std::string number = std::to_string(index);
		if (ImGui::BeginTabItem(number.c_str())) {
			const char* modes[] = { "NONE", "LAMBERT", "HALFLAMBERT" };
			ImGui::Combo("LightingMode", &lightData_->pointLight[index].type, modes, IM_ARRAYSIZE(modes));
			ImGui::ColorEdit3("color", &lightData_->pointLight[index].color.x);
			ImGui::SliderFloat3("position", &lightData_->pointLight[index].position.x, -10, 10);
			ImGui::SliderFloat("intensity", &lightData_->pointLight[index].intensity, 0.0f, 1.0f);
			ImGui::SliderFloat("radius", &lightData_->pointLight[index].radius, 0, 100);
			ImGui::SliderFloat("decay", &lightData_->pointLight[index].decay, 0, 100);
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();
	ImGui::End();

	ImGui::Begin("SpotLight");
	ImGui::BeginTabBar("SpotLight");
	for (int index = 0; index < kSpotLightNum; index++) {
		std::string number = std::to_string(index);
		if (ImGui::BeginTabItem(number.c_str())) {
			const char* modes[] = { "NONE", "LAMBERT", "HALFLAMBERT" };
			ImGui::Combo("LightingMode", &lightData_->spotLight[index].lightingType, modes, IM_ARRAYSIZE(modes));
			ImGui::ColorEdit3("color", &lightData_->spotLight[index].color.x);
			ImGui::SliderFloat3("position", &lightData_->spotLight[index].position.x, -10, 10);
			ImGui::SliderFloat("intensity", &lightData_->spotLight[index].intensity, 0.0f, 50.0f);
			ImGui::SliderFloat3("direction", &lightData_->spotLight[index].direction.x, -1.0f, 1.0f);
			ImGui::SliderFloat("distance", &lightData_->spotLight[index].distance, 0, 100);
			ImGui::SliderFloat("decay", &lightData_->spotLight[index].decay, 0, 10);
			ImGui::SliderFloat("cosAngle", &lightData_->spotLight[index].cosAngle, 0, 1);
			ImGui::SliderFloat("falloffStart", &lightData_->spotLight[index].falloffStart, lightData_->spotLight[index].cosAngle + 0.01f, 1);
			ImGui::EndTabItem();
		}

		lightData_->spotLight[index].direction = Normalize(lightData_->spotLight[index].direction);
		if (std::isnan(lightData_->spotLight[index].direction.x)) {
			lightData_->spotLight[index].direction = { 0.0f, -1.0f, 0.0f };
		}
	}
	ImGui::EndTabBar();
	ImGui::End();

#endif // _DEBUG
}

void LightObject::Draw() {
	//パイプラインステートの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetPipelineState(GraphicsPipelineManager::GetInstance()->GetPSO(kDefault));
	//ルートシグネチャの設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootSignature(GraphicsPipelineManager::GetInstance()->GetRootSignature(kDefault));
	//ライトの設定をコマンドリストに積む
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, lightResource_->GetGPUVirtualAddress());
}