#include "SrvManager.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

const uint32_t SrvManager::kMaxCount_ = 4096;

SrvManager* SrvManager::GetInstance() {
	static SrvManager instance;
	return &instance;
}

void SrvManager::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();

	//ディスクリプターヒープの生成
	descriptorHeap_ = dxCommon_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxCount_, true);
	
	descriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

uint32_t SrvManager::Allocate() {
	for (int index = 0; index < kMaxCount_; index++) {
		assert(index < kMaxCount_);
		if (!isUseIndex_[index]) {
			isUseIndex_[index] = true;
			return index;
		}
	}

	//SRVの確保失敗
	assert(false);
	return NULL;
}

void SrvManager::UnLoadResource(int index) {
	isUseIndex_[index] = false;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(int index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += descriptorSize_ * index;
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(int index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += descriptorSize_ * index;
	return handleGPU;
}