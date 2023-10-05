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

	//フェンスの生成
	CreateFence();
	
	//DXCの初期化
	InitializeDXC();


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
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	//画面のクリア
	ClearRenderTarget();

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
	}

	//次のフレーム用のコマンドリストの準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DirectXCommon::ReleaseCheck() {
	//リソースリークチェック
	ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}
}

IDxcBlob* DirectXCommon::CompilerShader(const std::wstring& filePath, const wchar_t* profile) {
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
		//やばいエラー時に止める
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止める
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止める
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

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
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc.NumDescriptors = 2;
	LRESULT hr = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap_));
	//ディスクリプターヒープの生成ができなかった場合に落とす
	assert(SUCCEEDED(hr));

	DXGI_SWAP_CHAIN_DESC swcDesc{};
	hr = swapChain_->GetDesc(&swcDesc);
	assert(SUCCEEDED(hr));
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++) {

		//バッファの取得
		hr = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		assert(SUCCEEDED(hr));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), i,
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		
		//RTVの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		//RTVの生成
		device_->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
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