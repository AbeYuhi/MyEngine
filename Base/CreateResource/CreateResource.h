#pragma once
#include "../DirectXCommon/DirectXCommon.h"
#include "../WinApp/WinApp.h"

/// <summary>
/// 頂点のメモリ確保
/// </summary>
ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);