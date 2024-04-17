#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "Math/Vector4.h"

/// <summary>
/// 頂点のメモリ確保
/// </summary>
ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

ComPtr<ID3D12Resource> CreateRenderTextureResoruce(uint32_t width, uint32_t height,	DXGI_FORMAT format, const Vector4& clearColor);