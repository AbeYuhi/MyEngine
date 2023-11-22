#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// 頂点のメモリ確保
/// </summary>
ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);