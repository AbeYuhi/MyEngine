#include "GraphicsPipelineManager.h"

GraphicsPipelineManager* GraphicsPipelineManager::GetInstance() {
	static GraphicsPipelineManager instance;
	return &instance;
}

void GraphicsPipelineManager::Initialize() {

	blendMode_ = BlendMode::kBlendModeNone;

	CreateGraphicsPipelineManager();

}

void GraphicsPipelineManager::CreateGraphicsPipelineManager() {

	//ルートシグネチャーの生成
	CreateRootSignature();

	//PSOの生成
	CreatePSO();

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	//ビューポート
	viewPort_.Width = (float)WinApp::GetInstance()->kWindowWidth;
	viewPort_.Height = (float)WinApp::GetInstance()->kWindowHeight;
	viewPort_.TopLeftX = 0;
	viewPort_.TopLeftY = 0;
	viewPort_.MinDepth = 0.0f;
	viewPort_.MaxDepth = 1.0f;

	//シザー短形
	scissorRect_.left = 0;
	scissorRect_.right = WinApp::GetInstance()->kWindowWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = WinApp::GetInstance()->kWindowHeight;
}

void GraphicsPipelineManager::CreateRootSignature() {
	//DirectXCommonの取得
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	ComPtr<ID3DBlob> signatureBlob[PipelineState::kCountOfPipelineState];
	ComPtr<ID3DBlob> errorBlob[PipelineState::kCountOfPipelineState];
	for (int shaderPack = 0; shaderPack < PipelineState::kCountOfPipelineState; shaderPack++) {
		switch (shaderPack)
		{
		case PipelineState::kDefault:
		case PipelineState::kWireFrame:
		default:
#pragma region 通常のシェーダー
		{
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
			D3D12_ROOT_PARAMETER rootParameters[5] = {};
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].Descriptor.ShaderRegister = 0;
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[1].Descriptor.ShaderRegister = 0;
			rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
			rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
			rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[3].Descriptor.ShaderRegister = 1;
			rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[4].Descriptor.ShaderRegister = 2;
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
#pragma endregion
			break;
		case PipelineState::kParticle:
#pragma region パーティクルシェーダー
		{
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
			D3D12_ROOT_PARAMETER rootParameters[3] = {};
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[0].Descriptor.ShaderRegister = 0;
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange;
			rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
			rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
			rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
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
#pragma endregion
			break;
		}
	}
}

void GraphicsPipelineManager::CreatePSO() {
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//InputLayoutの設定
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc[BlendMode::kCountOfBlendMode] = {0};
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
	ComPtr<IDxcBlob> vertexShaderBlob[PipelineState::kCountOfPipelineState]{0};
	ComPtr<IDxcBlob> pixelShaderBlob[PipelineState::kCountOfPipelineState]{0};
	for (int shaderPack = 0; shaderPack < PipelineState::kCountOfPipelineState; shaderPack++) {
		switch (shaderPack)
		{
		case PipelineState::kDefault:
		case PipelineState::kWireFrame:
		default:
			//頂点シェーダー
			vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/Object3D.VS.hlsl", L"vs_6_0");
			assert(vertexShaderBlob[shaderPack] != nullptr);
			//ピクセルシェーダー
			pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/Object3D.PS.hlsl", L"ps_6_0");
			assert(pixelShaderBlob[shaderPack] != nullptr);
			break;
		case PipelineState::kParticle:
			vertexShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/Particle.VS.hlsl", L"vs_6_0");
			assert(vertexShaderBlob[shaderPack] != nullptr);
			//ピクセルシェーダー
			pixelShaderBlob[shaderPack] = directXCommon->CompilerShader(L"Resources/Shaders/Particle.PS.hlsl", L"ps_6_0");
			assert(pixelShaderBlob[shaderPack] != nullptr);
			break;
		}
	}

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc[PipelineState::kCountOfPipelineState]{};
	for (int shaderPack = 0; shaderPack < PipelineState::kCountOfPipelineState; shaderPack++) {
		switch (shaderPack)
		{
		case PipelineState::kDefault:
		case PipelineState::kWireFrame:
		default:
			depthStencilDesc[shaderPack].DepthEnable = true;
			depthStencilDesc[shaderPack].DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc[shaderPack].DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			break;
		case PipelineState::kParticle:
			depthStencilDesc[shaderPack].DepthEnable = true;
			depthStencilDesc[shaderPack].DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			depthStencilDesc[shaderPack].DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			break;
		}
	}

	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeLineStateDesc{};
	for (int shaderPack = 0; shaderPack < PipelineState::kCountOfPipelineState; shaderPack++) {
		for (int blendMode = 0; blendMode < BlendMode::kCountOfBlendMode; blendMode++) {
			graphicsPipeLineStateDesc.pRootSignature = rootSignature_[shaderPack].Get();
			graphicsPipeLineStateDesc.InputLayout = inputLayoutDesc;
			graphicsPipeLineStateDesc.VS = { vertexShaderBlob[shaderPack]->GetBufferPointer(), vertexShaderBlob[shaderPack]->GetBufferSize() };
			graphicsPipeLineStateDesc.PS = { pixelShaderBlob[shaderPack]->GetBufferPointer(), pixelShaderBlob[shaderPack]->GetBufferSize() };
			graphicsPipeLineStateDesc.BlendState = blendDesc[blendMode];
			graphicsPipeLineStateDesc.RasterizerState = rasterizerDesc;
			graphicsPipeLineStateDesc.DepthStencilState = depthStencilDesc[shaderPack];
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