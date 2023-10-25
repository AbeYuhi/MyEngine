#pragma once
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"

class GraphicsPipelineManager {
public: //静的関数
	static GraphicsPipelineManager* GetInstance();

public: //メンバ関数

	void Initialize();

public: //ゲッターセッター

	inline ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }
	inline ID3D12PipelineState* GetPSO() { return graphicsPipelineState_.Get(); }
	inline D3D12_VIEWPORT* GetViewPort() { return &viewPort_; }
	inline D3D12_RECT* GetScissorRect() { return &scissorRect_; }

private:
	GraphicsPipelineManager() = default;
	~GraphicsPipelineManager() = default;

	void CreateGraphicsPipelineManager();

	/// <summary>
	///	ルートシグネチャーの生成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// PSOの生成
	/// </summary>
	void CreatePSO();

private: //メンバ変数
	ComPtr<ID3D12RootSignature> rootSignature_;
	ComPtr<ID3D12PipelineState> graphicsPipelineState_;
	D3D12_VIEWPORT viewPort_;
	D3D12_RECT scissorRect_;
};