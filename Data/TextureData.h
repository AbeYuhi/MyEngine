#pragma once
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/CreateResource/CreateResource.h"
#include "External/DirectXTex/DirectXTex.h"
#include "External/DirectXTex/d3dx12.h"

struct TextureData {
	DirectX::ScratchImage mipImage;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	ComPtr<ID3D12Resource> textureResource;
};