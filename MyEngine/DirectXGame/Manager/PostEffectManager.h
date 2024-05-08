#pragma once
#include "Base/DirectXCommon/DirectXCommon.h"
#include "GraphicsPipelineManager.h"
#include "ImGuiManager.h"

enum PostEffect {
	kNone,
	kCopy,
	kCountOfPostEffect
};

class PostEffectManager
{
public: //静的関数
	static PostEffectManager* GetInstance();

public: //メンバ関数

	void Initialize();

	void Draw();

private:
	PostEffectManager() = default;
	~PostEffectManager() = default;

	void CreateRenderTexture();

	void CreateRootSignature();

	void CreatePSO();

	ComPtr<ID3D12Resource>CreateRenderTextureResoruce(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

private: //メンバ変数
	ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature_[PostEffect::kCountOfPostEffect];
	ComPtr<ID3D12PipelineState> graphicsPipelineState_[PostEffect::kCountOfPostEffect][BlendMode::kCountOfBlendMode];
	PostEffect postEffect_;
};

