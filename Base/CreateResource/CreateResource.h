#pragma once
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// 頂点のメモリ確保
/// </summary>
ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);