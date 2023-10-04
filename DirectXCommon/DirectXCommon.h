#pragma once
#include "../WinApp/WinApp.h"
#include "../Debug/DebugLog.h"
#include <WRL/client.h>
#include <format>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <cassert>

using namespace Microsoft::WRL;

class DirectXCommon {
public: //静的関数
	static DirectXCommon* GetInstance();

public: //静的変数


public: //メンバ関数

	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

public: //ゲッターセッター

	inline ID3D12Device* GetDevice() { device_.Get(); }

	inline ID3D12CommandList* GetCommandList() { commandList_.Get(); }

private: //メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;

	void InitializeDxgiDevice();

	void InitializeCommand();

	void InitializeSwapChain();

	void InitializeRenderTargetView();

private: //メンバ変数
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
};