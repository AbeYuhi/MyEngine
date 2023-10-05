#pragma once
#include "../WinApp/WinApp.h"
#include "../Debug/DebugLog.h"
#include <WRL/client.h>
#include <format>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
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

	/// <summary>
	/// 画面のクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 解放チェック
	/// </summary>
	void ReleaseCheck();

	/// <summary>
	/// Dxcのコンパイルシェーダー
	/// </summary>
	IDxcBlob* CompilerShader(const std::wstring& filePath, const wchar_t* profile);

public: //ゲッターセッター

	inline ID3D12Device* GetDevice() { return device_.Get(); }

	inline ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private: //メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;

	void InitializeDxgiDevice();

	void InitializeCommand();

	void InitializeSwapChain();

	void CreateRenderTargetView();

	void CreateFence();

	void InitializeDXC();

private: //メンバ変数
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Fence> fence_ = nullptr;
	ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
	uint64_t fenceValue_ = 0;
};