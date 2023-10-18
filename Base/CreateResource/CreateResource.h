#pragma once
#include "../DirectXCommon/DirectXCommon.h"
#include "../WinApp/WinApp.h"

/// <summary>
/// 頂点のメモリ確保
/// </summary>
ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);


/// <summary>
/// 深度情報を収納するためのリソース確保関数
/// </summary>
/// <param name="width">画像横幅</param>
/// <param name="height">画像縦幅</param>
/// <returns></returns>
ComPtr<ID3D12Resource> CreateDepthStencilTextureResource();