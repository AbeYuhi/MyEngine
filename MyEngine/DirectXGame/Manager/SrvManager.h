#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "External/DirectXTex/d3dx12.h"
#include <WRL/client.h>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <cassert>
#include <map>

class DirectXCommon;

class SrvManager
{
public:
	static SrvManager* GetInstance();

public:

	void Initialize();

	uint32_t Allocate();

	inline ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return descriptorHeap_.Get(); }

	void UnLoadResource(int index);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(int index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(int index);

private:
	SrvManager() = default;
	~SrvManager() = default;
	
private:
	DirectXCommon* dxCommon_;
	static const uint32_t kMaxCount_;
	uint32_t descriptorSize_;
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;
	std::map<uint32_t, bool> isUseIndex_;
};

