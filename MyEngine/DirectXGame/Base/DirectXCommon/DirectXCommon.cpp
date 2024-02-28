#include "DirectXCommon.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

DirectXCommon* DirectXCommon::GetInstance() {
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::Initialize() {

	//DxgiDeviceの初期化
	InitializeDxgiDevice();

	//コマンドリストの初期化
	InitializeCommand();

	//スワップチェーンの初期化
	InitializeSwapChain();

	//RTVの生成
	CreateRenderTargetView();

	//SRVの生成
	CreateShaderResourceView();

	//DSVの生成
	CreateDepthStencilView();

	//フェンスの生成
	CreateFence();
	
	//DXCの初期化
	InitializeDXC();

	//FPS固定初期化
	InitializeFixFPS();

}

void DirectXCommon::PreDraw() {
	//バックバッファの取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	
	//バックバッファのハンドルの取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), backBufferIndex,
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアの種類
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = backBuffers[backBufferIndex].Get();
	//遷移前のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResoruceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);

	//RTVの設定
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//画面のクリア
	ClearRenderTarget();

	//描画用DescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { DirectXCommon::GetInstance()->GetSrvDescriptorHeap() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);
}

void DirectXCommon::PostDraw() {
	//バックバッファの取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	//バックバッファのハンドルの取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), backBufferIndex,
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	//Barrierの状態遷移
	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアの種類
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = backBuffers[backBufferIndex].Get();
	//遷移前のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//遷移後のResoruceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier);

	//コマンドリストを閉じる
	LRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = {commandList_.Get()};
	commandQueue_->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行わせる
	swapChain_->Present(1, 0);

	///同期処理
	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまでたどり着いたら、Fenceの値を更新した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), fenceValue_);
	//Fenceの値がSignal値にになったかを確認
	if (fence_->GetCompletedValue() < fenceValue_) {
		//イベントの生成
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);
		//指定したSignalにたどり着いてないので待つようにイベントを設定
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
		CloseHandle(fenceEvent);
	}

	//FPS固定
	UpdateFixFPS();

	//次のフレーム用のコマンドリストの準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

ComPtr<IDxcBlob> DirectXCommon::CompilerShader(const std::wstring& filePath, const wchar_t* profile) {
	//これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, Path:{}, profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//コンパイル
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E", L"main",
		L"-T", profile,
		L"-Zi", L"-Qembed_debug",
		L"-Od",
		L"-Zpr"
	};
	//実際にShaderのコンパイルをする
	ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler_.Get(),
		IID_PPV_ARGS(&shaderResult));
	//コンパイルエラーチェック
	assert(SUCCEEDED(hr));

	//警告・エラーが出たら停止
	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		//警告・エラーダメ絶対
		assert(false);
	}

	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, Path:{}, profile:{}\n", filePath, profile)));

	//実行用のバイナリを返却
	return shaderBlob;
}

ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible) {
	//ディスクリプターヒープの生成
	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = heapType;
	rtvDescriptorHeapDesc.NumDescriptors = numDescriptors;
	rtvDescriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	LRESULT hr = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	//ディスクリプターヒープの生成ができなかった場合に落とす
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}

ComPtr<ID3D12Resource> DirectXCommon::CreateDepthStencilTextureResource() {
	//WinAppの取得
	WinApp* winApp = WinApp::GetInstance();
	//DxCommonの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = winApp->kWindowWidth;
	resourceDesc.Height = winApp->kWindowHeight;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//利用するヒープの設定
	D3D12_HEAP_PROPERTIES heapProperities{};
	heapProperities.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深層値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProperities,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

void DirectXCommon::ClearRenderTarget() {
	//バックバッファの取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	//バックバッファのハンドルの取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), backBufferIndex,
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	//指定した色で画面のクリア
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	//深度バッファのクリア
	ClearDepthStencilBuffer();
}

void DirectXCommon::ClearDepthStencilBuffer() {
	//指定した深度で画面をクリア
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DirectXCommon::TransferCommandList() {
	//コマンドリストを閉じる
	LRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	///同期処理
	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまでたどり着いたら、Fenceの値を更新した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_.Get(), fenceValue_);
	//Fenceの値がSignal値にになったかを確認
	if (fence_->GetCompletedValue() < fenceValue_) {
		//イベントの生成
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);
		//指定したSignalにたどり着いてないので待つようにイベントを設定
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	//次のフレーム用のコマンドリストの準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DirectXCommon::InitializeDxgiDevice() {
	//デバックレイヤー
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバックレイヤーの有効化
		debugController->EnableDebugLayer();
		//GPU側でもチェックするように
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG

	
	//DxgiFactoryの生成
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr));

	//使用するアダプタの決定
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter = nullptr;
	for(UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプターの情報の取得
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));
		//ソフトウェアアダプタじゃなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			Log(ConvertString(std::format(L"Use adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;
	}
	//アダプターが見つからなかった場合落とさせる
	assert(useAdapter != nullptr);

	//D3D12Deviceの生成
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = {"12.2", "12.1", "12.0"};
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); i++) {
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		if (SUCCEEDED(hr)) {
			//生成できたのでログに出力しループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスの生成がうまくいかなかったら落とす
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!\n");

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		bool isStop = false;
		//やばいエラー時に止める
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, isStop);
		//エラー時に止める
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, isStop);
		//警告時に止める
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, isStop);

		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = {D3D12_MESSAGE_SEVERITY_INFO};
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示の抑制
		infoQueue->PushStorageFilter(&filter);
	}
#endif // _DEBUG

}

void DirectXCommon::InitializeCommand() {
	//コマンドキューの生成
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	HRESULT hr = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成ができなかった場合落とす
	assert(SUCCEEDED(hr));

	//コマンドアロケーターの生成
	hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	//コマンドアロケーターの生成ができなかった場合落とす
	assert(SUCCEEDED(hr));

	//コマンドリストの生成
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	//コマンドリストの生成の生成ができなかった場合落とす
	assert(SUCCEEDED(hr));
}

void DirectXCommon::InitializeSwapChain() {
	WinApp* winApp = WinApp::GetInstance();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = winApp->kWindowWidth;
	swapChainDesc.Height = winApp->kWindowHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
	LRESULT hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), winApp->GetHWND(), &swapChainDesc, nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(hr));
	//SwapChain4の取得
	hr = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));
	assert(SUCCEEDED(hr));
}

void DirectXCommon::CreateRenderTargetView() {
	//ディスクリプターヒープの生成
	rtvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);

	//RTVの設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	DXGI_SWAP_CHAIN_DESC swcDesc{};
	HRESULT hr = swapChain_->GetDesc(&swcDesc);
	assert(SUCCEEDED(hr));
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++) {

		//バッファの取得
		hr = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		assert(SUCCEEDED(hr));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), i,
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

		//RTVの生成
		device_->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc_, rtvHandle);
	}
}

void DirectXCommon::CreateShaderResourceView() {
	//ディスクリプターヒープの生成
	srvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2056, true);
}

void DirectXCommon::CreateDepthStencilView() {
	//DSVディスクリプターヒープの生成
	dsvDescriptorHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	//DSVディスクリプターの設定
	dsvDesc_.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc_.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//depthStencilResourceの生成
	depthStencilResource_ = CreateDepthStencilTextureResource();

	//DsvHeapの先頭にDSVの生成
	device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc_, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::CreateFence() {
	//初期値0でFenceの生成
	fenceValue_ = 0;
	LRESULT hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));
}

void DirectXCommon::InitializeDXC() {
	LRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	//現時点ではincludeはしないが、includeに対応するための設定を行っておく
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

void DirectXCommon::InitializeFixFPS() {
	WinApp* winApp = WinApp::GetInstance();
	//現在時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS() {
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//現在時間を取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//前回記録からの経過時間を取得
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒(よりわずかに短い時間)経ってない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			//1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//現在の時間を記録
	reference_ = std::chrono::steady_clock::now();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetRtvHandle() {
	//バックバッファの取得
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();

	//バックバッファのハンドルの取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
	rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), backBufferIndex,
	device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	return rtvHandle;
}

D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetCPUDescriptorHandle(int index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXCommon::GetGPUDescriptorHandle(int index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;
	return handleGPU;
}