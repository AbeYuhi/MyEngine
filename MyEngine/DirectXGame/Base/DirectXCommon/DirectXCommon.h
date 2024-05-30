#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/DebugSytem/DebugLog.h"
#include "Math/Vector4.h"
#include "External/DirectXTex/d3dx12.h"
#include <WRL/client.h>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <cassert>
#include <compare>
#include <chrono>
#include <thread>

using namespace Microsoft::WRL;

class SrvManager;

class DirectXCommon {
public: //静的関数
	static DirectXCommon* GetInstance();

public: //メンバ関数

	void Initialize();

	void PostDraw();

	/// <summary>
	/// 画面のクリア
	/// </summary>
	void ClearRenderTarget(Vector4 clearScreenColor, CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle);

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthStencilBuffer();

	/// <summary>
	/// コマンドのキックから次のコマンドリストの準備
	/// </summary>
	void TransferCommandList();

	/// <summary>
	/// コマンドリストのキック、スワップチェインの実行
	/// </summary>
	void SwapChainProcessing();

	/// <summary>
	/// FPSの固定化
	/// </summary>
	void UpdateFixFPS();

	/// <summary>
	/// Dxcのコンパイルシェーダー
	/// </summary>
	ComPtr<IDxcBlob> CompilerShader(const std::wstring& filePath, const wchar_t* profile);

	/// <summary>
	/// ディスクリプターヒープの生成関数
	/// </summary>
	/// <param name="heapType"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaderVisible"></param>
	/// <returns></returns>
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// 深度情報を収納するためのリソース確保関数
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12Resource> CreateDepthStencilTextureResource();

public: //ゲッターセッター

	inline ID3D12Device* GetDevice() { return device_.Get(); }

	inline ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	inline IDXGISwapChain4* GetSwapShain() { return swapChain_.Get(); }

	inline ID3D12DescriptorHeap* GetRtvDescriptorHeap() { return rtvDescriptorHeap_.Get(); }

	inline ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return dsvDescriptorHeap_.Get(); }

	inline D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc_; }

	inline D3D12_DEPTH_STENCIL_VIEW_DESC GetDsvDesc() { return dsvDesc_; }

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle();

	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle();

	inline ID3D12Resource* GetBackBuffer(int index) { return backBuffers[index].Get(); }

	/// <summary>
	/// RTVハンドルの取得
	/// 0～1 : スワップチェイン
	/// 2 : レンダーテクスチャー
	/// </summary>
	/// <param name="index">
	/// </param>
	/// <returns></returns>
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle(int index);

private: //メンバ関数
	DirectXCommon() = default;
	~DirectXCommon() = default;

	void InitializeDxgiDevice();

	void InitializeCommand();

	void InitializeSwapChain();

	void CreateRenderTargetView();

	void CreateShaderResourceView();

	void CreateDepthStencilView();

	void CreateFence();

	void InitializeDXC();

	void InitializeFixFPS();

private: //メンバ変数
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_;
	SrvManager* srvManager_;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Fence> fence_ = nullptr;
	ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	uint64_t fenceValue_ = 0;
	std::chrono::steady_clock::time_point reference_;
};