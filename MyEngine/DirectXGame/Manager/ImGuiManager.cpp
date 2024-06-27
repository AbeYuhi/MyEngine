#include "ImGuiManager.h"

#ifdef _DEBUG

ImGuiManager* ImGuiManager::GetInstance() {
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize() {
	WinApp* winApp = WinApp::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	SrvManager* srvManager = SrvManager::GetInstance();
	uint32_t index = srvManager->Allocate();
	DXGI_SWAP_CHAIN_DESC1 swapChain;
	dxCommon->GetSwapShain()->GetDesc1(&swapChain);
	IMGUI_CHECKVERSION();
	CreateContext();
	StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHWND());
	ImGui_ImplDX12_Init(dxCommon->GetDevice(),
		swapChain.BufferCount,
		dxCommon->GetRtvDesc().Format,
		srvManager->GetSrvDescriptorHeap(),
		srvManager->GetCPUDescriptorHandle(index),
		srvManager->GetGPUDescriptorHandle(index));
}

void ImGuiManager::Begin() {
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
}

void ImGuiManager::End() {
	Render();
}

void ImGuiManager::Draw() {
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(GetDrawData(), commandList);
}

void ImGuiManager::ReleseProcess() {
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::RenderItemDebug(std::string name, RenderItem& renderItem) {
	if (ImGui::BeginTabItem(name.c_str())) {
		ImGui::SliderFloat3("pos", &renderItem.worldTransform_.data_.translate_.x, -10, 10);
		ImGui::SliderFloat3("rotate", &renderItem.worldTransform_.data_.rotate_.x, -2 * 3.14f, 2 * 3.14f);
		ImGui::SliderFloat3("scale", &renderItem.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat3("UvPos", &renderItem.materialInfo_.uvTransform_.translate_.x, -10, 10);
		ImGui::SliderFloat3("UvRotate", &renderItem.materialInfo_.uvTransform_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("UvScale", &renderItem.materialInfo_.uvTransform_.scale_.x, -10, 10);
		ImGui::SliderFloat("shininess", &renderItem.materialInfo_.material_->shininess, 0, 100);

		bool enableLightint = renderItem.materialInfo_.material_->enableLightint;
		ImGui::Checkbox("enableLight", &enableLightint);
		renderItem.materialInfo_.material_->enableLightint = enableLightint;

		bool isSpecularReflection = renderItem.materialInfo_.material_->isSpecularReflection;
		ImGui::Checkbox("isSpecularReflection", &isSpecularReflection);
		renderItem.materialInfo_.material_->isSpecularReflection = isSpecularReflection;

		ImGui::ColorEdit4("color", &renderItem.materialInfo_.material_->color.x);
		ImGui::ColorEdit3("shinessColor", &renderItem.materialInfo_.material_->shininessColor.x);

		for (auto it = renderItem.animation_.infos.begin(); it != renderItem.animation_.infos.end(); it++) {
			ImGui::Checkbox(it->data.name.c_str(), &it->isAnimation);
			std::string animationSpeed = it->data.name + ": speed";
			ImGui::SliderFloat(animationSpeed.c_str(), &it->animationSpeed, -5.0f, 5.0f);
			std::string animationLoop = it->data.name + ": loop";
			ImGui::Checkbox(animationLoop.c_str(), &it->isLoop);
		}

		ImGui::EndTabItem();
	}
}

void ImGuiManager::SpriteItemDebug(std::string name, SpriteItem& spriteItem) {
	if (ImGui::BeginTabItem(name.c_str())) {
		ImGui::SliderFloat3("pos", &spriteItem.worldTransform_.data_.translate_.x, -1000, 1000);
		ImGui::SliderFloat3("rotate", &spriteItem.worldTransform_.data_.rotate_.x, -2 * 3.14f, 2 * 3.14f);
		ImGui::SliderFloat3("scale", &spriteItem.worldTransform_.data_.scale_.x, -10, 10);
		ImGui::SliderFloat3("UvPos", &spriteItem.materialInfo_.uvTransform_.translate_.x, -10, 10);
		ImGui::SliderFloat3("UvRotate", &spriteItem.materialInfo_.uvTransform_.rotate_.x, -10, 10);
		ImGui::SliderFloat3("UvScale", &spriteItem.materialInfo_.uvTransform_.scale_.x, -10, 10);
		ImGui::SliderFloat2("spriteSize", &spriteItem.spriteData_.size_.x, 0, 360);
		ImGui::SliderFloat2("anchorPoint", &spriteItem.spriteData_.anchorPoint_.x, 0, 1);
		ImGui::SliderFloat2("baseUvPos", &spriteItem.spriteData_.baseUvPos_.x, 0, 1);
		ImGui::SliderFloat2("texSize", &spriteItem.spriteData_.texSize_.x, -10, 10);
		ImGui::Checkbox("isFlipX", &spriteItem.spriteData_.isFlipX_);
		ImGui::Checkbox("isFlipY", &spriteItem.spriteData_.isFlipY_);

		ImGui::ColorEdit4("color", &spriteItem.materialInfo_.material_->color.x);

		ImGui::EndTabItem();
	}
}

#endif // _DEBUG