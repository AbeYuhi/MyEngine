#include "ImGuiManager.h"

#ifdef _DEBUG

ImGuiManager* ImGuiManager::GetInstance() {
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize() {
	WinApp* winApp = WinApp::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	DXGI_SWAP_CHAIN_DESC1 swapChain;
	dxCommon->GetSwapShain()->GetDesc1(&swapChain);
	IMGUI_CHECKVERSION();
	CreateContext();
	StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHWND());
	ImGui_ImplDX12_Init(dxCommon->GetDevice(),
		swapChain.BufferCount,
		dxCommon->GetRtvDesc().Format,
		dxCommon->GetSrvDescriptorHeap(),
		dxCommon->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
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

#endif // _DEBUG