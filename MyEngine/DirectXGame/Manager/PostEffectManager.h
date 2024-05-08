#pragma once
#include "Base/DirectXCommon/DirectXCommon.h"
#include "GraphicsPipelineManager.h"

class PostEffectManager
{
public: //静的関数
	static PostEffectManager* GetInstance();

public: //メンバ関数

	void Initialize();

private:
	PostEffectManager() = default;
	~PostEffectManager() = default;

	void CreateRenderTexture();

	ComPtr<ID3D12Resource>CreateRenderTextureResoruce(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

private: //メンバ変数

	ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
};

