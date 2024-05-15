#include "PostEffectManager.h"

PostEffectManager* PostEffectManager::GetInstance() {
	static PostEffectManager instance;
	return &instance;
}

void PostEffectManager::Initialize() {

	postEffect_ = kNone;

	//Rendertextureの生成
	CreateRenderTexture();

	//ルートシグネチャーの生成
	CreateRootSignature();

	//PSOの生成
	CreatePSO();

}

void PostEffectManager::PreDraw() {
	DirectXCommon* directX = DirectXCommon::GetInstance();

	if (postEffect_ == PostEffect::kNone) {
		NormalPreDraw();
		directX->ClearDepthStencilBuffer();
	}
	else{
		RenderPreDraw();
	}
}

void PostEffectManager::PostDraw() {
	DirectXCommon* directX = DirectXCommon::GetInstance();

	if (postEffect_ != PostEffect::kNone) {
		RenderPostDraw();
	}
}

void PostEffectManager::NormalPreDraw() {
	DirectXCommon* directX = DirectXCommon::GetInstance();

	//バックバッファの取得
	UINT backBufferIndex = directX->GetSwapShain()->GetCurrentBackBufferIndex();

	//バックバッファのハンドルの取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = directX->GetRtvHandle(backBufferIndex);

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアの種類
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = directX->GetBackBuffer(backBufferIndex);
	//遷移前のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResoruceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	directX->GetCommandList()->ResourceBarrier(1, &barrier);

	//RTVの設定
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = directX->GetDsvHandle();
	directX->GetCommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//画面のクリア
	directX->ClearRenderTarget({ 0.1f, 0.25f, 0.5f, 1.0f }, rtvHandle);

	//描画用DescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { DirectXCommon::GetInstance()->GetSrvDescriptorHeap() };
	directX->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void PostEffectManager::RenderPreDraw() {
	DirectXCommon* directX = DirectXCommon::GetInstance();

	//バックバッファのハンドルの取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = directX->GetRtvHandle(kRTVIndex);

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアの種類
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = renderTextureResource_.Get();
	//遷移前のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//遷移後のResoruceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//TransitionBarrierを張る
	directX->GetCommandList()->ResourceBarrier(1, &barrier);

	//RTVの設定
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = directX->GetDsvHandle();
	directX->GetCommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//画面のクリア
	//指定した色で画面のクリア
	directX->ClearRenderTarget({ 1.0f, 0.0f, 0.0f, 1.0f }, rtvHandle);
	//深度バッファのクリア
	directX->ClearDepthStencilBuffer();

	//描画用DescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { DirectXCommon::GetInstance()->GetSrvDescriptorHeap() };
	directX->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void PostEffectManager::RenderPostDraw() {
	DirectXCommon* directX = DirectXCommon::GetInstance();

	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER renderBarrier{};
	//今回のバリアの種類
	renderBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	renderBarrier.Transition.pResource = renderTextureResource_.Get();
	//遷移前のResourceState
	renderBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//遷移後のResoruceState
	renderBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	directX->GetCommandList()->ResourceBarrier(1, &renderBarrier);

	//スワップチェインに表示するためセット
	NormalPreDraw();

	//描画処理の記入
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();
	directX->GetCommandList()->SetPipelineState(graphicsPipelineState_[postEffect_]->Get());
	directX->GetCommandList()->SetGraphicsRootSignature(rootSignature_[postEffect_].Get());
	directX->GetCommandList()->SetGraphicsRootDescriptorTable(0, directX->GetGPUDescriptorHandle(kSRVIndex));
	directX->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void PostEffectManager::CreateRenderTexture() {
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//RTVの生成
	const Vector4 kRenderTargetClearValue{ 1.0f, 0.0f, 0.0f, 1.0f }; //一旦赤色
	renderTextureResource_ = CreateRenderTextureResoruce(WinApp::kWindowWidth, WinApp::kWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = directXCommon->GetRtvHandle(kRTVIndex);
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = directXCommon->GetRtvDesc();
	directXCommon->GetDevice()->CreateRenderTargetView(renderTextureResource_.Get(), &rtvDesc, rtvHandle);

	//SRVの生成
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = directXCommon->GetCPUDescriptorHandle(kSRVIndex);
	directXCommon->GetDevice()->CreateShaderResourceView(renderTextureResource_.Get(), &renderTextureSrvDesc, handleCPU);
}

void PostEffectManager::CreateRootSignature() {
	//DirectXCommonの取得
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	ComPtr<ID3DBlob> signatureBlob[PostEffect::kCountOfPostEffect];
	ComPtr<ID3DBlob> errorBlob[PostEffect::kCountOfPostEffect];

	for (int shaderPack = 0; shaderPack < PostEffect::kCountOfPostEffect; shaderPack++) {
		switch (shaderPack)
		{
		case PostEffect::kNone:
		case PostEffect::kCopy:
		default:
			//DescriptorRangeの設定
			D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
			descriptorRange[0].BaseShaderRegister = 0;
			descriptorRange[0].NumDescriptors = 1;
			descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			//RootSignature生成
			D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
			descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			//RootParameter作成。
			D3D12_ROOT_PARAMETER rootParameters[1] = {};
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;
			rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
			descriptionRootSignature.pParameters = rootParameters;
			descriptionRootSignature.NumParameters = _countof(rootParameters);

			//Sampler
			D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
			staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
			staticSamplers[0].ShaderRegister = 0;
			staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			descriptionRootSignature.pStaticSamplers = staticSamplers;
			descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

			//シリアライズしてバイナリする
			LRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob[shaderPack], &errorBlob[shaderPack]);
			if (FAILED(hr)) {
				Log(reinterpret_cast<char*>(errorBlob[shaderPack]->GetBufferPointer()));
				assert(false);
			}
			//バイナリをもとに生成
			hr = directXCommon->GetDevice()->CreateRootSignature(0, signatureBlob[shaderPack]->GetBufferPointer(), signatureBlob[shaderPack]->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[shaderPack]));
			assert(SUCCEEDED(hr));
		}
	}
}

void PostEffectManager::CreatePSO() {
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//InputLayoutの設定
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc[BlendMode::kCountOfBlendMode] = { 0 };
	for (int blendMode = 0; blendMode < BlendMode::kCountOfBlendMode; blendMode++) {
		switch (blendMode)
		{
		case BlendMode::kBlendModeNone:
			blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			break;
		case BlendMode::kBlendModeNormal:
			blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case BlendMode::kBlendModeAdd:
			blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case BlendMode::kBlendModeSubtract:
			blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case BlendMode::kBlendModeMultily:
			blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
			blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
			blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case BlendMode::kBlendModeScreen:
			blendDesc[blendMode].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc[blendMode].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[blendMode].RenderTarget[0].SrcBlend = D3D12_BLEND_INV_SRC_COLOR;
			blendDesc[blendMode].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc[blendMode].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc[blendMode].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		default:
			break;
		}
	}

	//RasiterzerStatesの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏側を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob[PostEffect::kCountOfPostEffect]{ 0 };
	ComPtr<IDxcBlob> pixelShaderBlob[PostEffect::kCountOfPostEffect]{ 0 };
	for (int shaderPack = 0; shaderPack < PostEffect::kCountOfPostEffect; shaderPack++) {
		switch (shaderPack)
		{
		case PostEffect::kNone:
		default:
		case PostEffect::kCopy:
			//頂点シェーダー
			vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
			assert(vertexShaderBlob[shaderPack] != nullptr);
			//ピクセルシェーダー
			pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/CopyImage.PS.hlsl", L"ps_6_0");
			assert(pixelShaderBlob[shaderPack] != nullptr);
			break;
		case PostEffect::kGrayScale:
			//頂点シェーダー
			vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
			assert(vertexShaderBlob[shaderPack] != nullptr);
			//ピクセルシェーダー
			pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/GrayScale.PS.hlsl", L"ps_6_0");
			assert(pixelShaderBlob[shaderPack] != nullptr);
			break;
		case PostEffect::kSepiaScale:
			//頂点シェーダー
			vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
			assert(vertexShaderBlob[shaderPack] != nullptr);
			//ピクセルシェーダー
			pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/SepiaScale.PS.hlsl", L"ps_6_0");
			assert(pixelShaderBlob[shaderPack] != nullptr);
			break;
		case PostEffect::kVignette:
			//頂点シェーダー
			vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
			assert(vertexShaderBlob[shaderPack] != nullptr);
			//ピクセルシェーダー
			pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/Vignette.PS.hlsl", L"ps_6_0");
			assert(pixelShaderBlob[shaderPack] != nullptr);
			break;
		case PostEffect::kSmoothing:
			//頂点シェーダー
			vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/FullScreen.VS.hlsl", L"vs_6_0");
			assert(vertexShaderBlob[shaderPack] != nullptr);
			//ピクセルシェーダー
			pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/PostEffect/BoxFilter.PS.hlsl", L"ps_6_0");
			assert(pixelShaderBlob[shaderPack] != nullptr);
			break;
		}
	}

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = false;

	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeLineStateDesc{};
	for (int shaderPack = 0; shaderPack < PostEffect::kCountOfPostEffect; shaderPack++) {
		for (int blendMode = 0; blendMode < BlendMode::kCountOfBlendMode; blendMode++) {
			graphicsPipeLineStateDesc.pRootSignature = rootSignature_[shaderPack].Get();
			graphicsPipeLineStateDesc.InputLayout = inputLayoutDesc;
			graphicsPipeLineStateDesc.VS = { vertexShaderBlob[shaderPack]->GetBufferPointer(), vertexShaderBlob[shaderPack]->GetBufferSize() };
			graphicsPipeLineStateDesc.PS = { pixelShaderBlob[shaderPack]->GetBufferPointer(), pixelShaderBlob[shaderPack]->GetBufferSize() };
			graphicsPipeLineStateDesc.BlendState = blendDesc[blendMode];
			graphicsPipeLineStateDesc.RasterizerState = rasterizerDesc;
			graphicsPipeLineStateDesc.DepthStencilState = depthStencilDesc;
			graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			//書き込むRTVの情報
			graphicsPipeLineStateDesc.NumRenderTargets = 1;
			graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			//利用する形状タイプ
			if (shaderPack == PipelineState::kWireFrame) {
				graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			}
			else {
				graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			}

			//どのように画面に色を打ち込むのかの設定
			graphicsPipeLineStateDesc.SampleDesc.Count = 1;
			graphicsPipeLineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
			//実際に生成
			LRESULT hr = directXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipeLineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_[shaderPack][blendMode]));
			assert(SUCCEEDED(hr));
		}
	}
}

ComPtr<ID3D12Resource> PostEffectManager::CreateRenderTextureResoruce(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//バックバッファリソース
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	//クリア値
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;
	//実際に生成
	ComPtr<ID3D12Resource> resource;
	LRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}