#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "External/DirectXTex/DirectXTex.h"
#include "External/DirectXTex/d3dx12.h"

struct TextureData {
	DirectX::ScratchImage mipImage;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	ComPtr<ID3D12Resource> textureResource;
	uint32_t textureHandleIndex;
};