#pragma once
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/CreateResource/CreateResource.h"
#include "GraphicsPipelineManager.h"
#include "Data/KernelSize.h"
#include "ImGuiManager.h"

enum PostEffect {
	kNone,
	kCopy,
	kGrayScale,
	kSepiaScale,
	kVignette,
	kSmoothing,
	kCountOfPostEffect
};

class PostEffectManager
{
public: //静的関数
	static PostEffectManager* GetInstance();

public: //メンバ関数

	void Initialize();

	void PreDraw();

	void PostDraw();

public: //ゲッターセッター

	inline PostEffect GetPostEffect() { return postEffect_; }
	inline void SetPostEffect(PostEffect postEffect) { postEffect_ = postEffect; }

	inline int GetKernelSize() { return kernelSize_->size; }
	inline void SetKernelSize(int kernelSize) { kernelSize_->size = kernelSize; }

private:
	PostEffectManager() = default;
	~PostEffectManager() = default;

	void CreateRenderTexture();

	void CreateRootSignature();

	void CreatePSO();

	void NormalPreDraw();

	void RenderPreDraw();

	void RenderPostDraw();

	ComPtr<ID3D12Resource>CreateRenderTextureResoruce(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

private: //メンバ変数
	ComPtr<ID3D12Resource> renderTextureResource_ = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature_[PostEffect::kCountOfPostEffect];
	ComPtr<ID3D12PipelineState> graphicsPipelineState_[PostEffect::kCountOfPostEffect][BlendMode::kCountOfBlendMode];
	PostEffect postEffect_;
	const int kRTVIndex = 2;
	const int kSRVIndex = 3001;

	//各ポストエフェクトにおける必要なResourceなど
	//Smoothing
	ComPtr<ID3D12Resource> kernelSizeResource_ = nullptr;
	KernelSize* kernelSize_;
};

