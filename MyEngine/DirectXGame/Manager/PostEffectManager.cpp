#include "PostEffectManager.h"

PostEffectManager* PostEffectManager::GetInstance() {
	static PostEffectManager instance;
	return &instance;
}

void PostEffectManager::Initialize() {



}


void PostEffectManager::CreateRenderTexture() {
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//RTVの生成
	const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f }; //一旦赤色
	renderTextureResource_ = CreateRenderTextureResoruce(WinApp::kWindowWidth, WinApp::kWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = directXCommon->GetRtvHandle(2);
	directXCommon->GetDevice()->CreateRenderTargetView(renderTextureResource_.Get(), &directXCommon->GetRtvDesc(), rtvHandle);

	//SRVの生成
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 2001;
	device_->CreateShaderResourceView(renderTextureResource_.Get(), &renderTextureSrvDesc, handleCPU);
}

ComPtr<ID3D12Resource> PostEffectManager::CreateRenderTextureResoruce(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//バックバッファリソース
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	//クリア値
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;
	//実際に生成
	ComPtr<ID3D12Resource> resource;
	LRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}