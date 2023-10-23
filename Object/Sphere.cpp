#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

/// <summary>
/// 静的メンバ変数の実体化
/// </summary>
ComPtr<ID3D12RootSignature> Sphere::sRootSignature_;
ComPtr<ID3D12PipelineState> Sphere::sGraphicsPipelineState_;
D3D12_VIEWPORT Sphere::sViewPort_;
D3D12_RECT Sphere::sScissorRect_;
ID3D12GraphicsCommandList* Sphere::sCommandList_ = nullptr;

void Sphere::StaticInitialize() {
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//ルートシグネチャーの生成
	CreateRootSignature();

	//PSOの生成
	CreatePSO();

	//ビューポート
	sViewPort_.Width = (float)WinApp::GetInstance()->kWindowWidth;
	sViewPort_.Height = (float)WinApp::GetInstance()->kWindowHeight;
	sViewPort_.TopLeftX = 0;
	sViewPort_.TopLeftY = 0;
	sViewPort_.MinDepth = 0.0f;
	sViewPort_.MaxDepth = 1.0f;

	//シザー短形
	sScissorRect_.left = 0;
	sScissorRect_.right = WinApp::GetInstance()->kWindowWidth;
	sScissorRect_.top = 0;
	sScissorRect_.bottom = WinApp::GetInstance()->kWindowHeight;
}

void Sphere::PreDraw(ID3D12GraphicsCommandList* commandList) {
	//1フレーム前にPostDrawが呼ばれていなかったらエラー
	assert(Sphere::sCommandList_ == nullptr);
	//コマンドリストのセット
	sCommandList_ = commandList;

	//ViewPortの設定
	sCommandList_->RSSetViewports(1, &sViewPort_);
	//Scirssorの設定
	sCommandList_->RSSetScissorRects(1, &sScissorRect_);
	//パイプラインステートの設定
	sCommandList_->SetPipelineState(sGraphicsPipelineState_.Get());
	//ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(sRootSignature_.Get());
	//プリミティブ形状を設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sphere::PostDraw() {
	//コマンドリストを解除
	sCommandList_ = nullptr;
}

std::unique_ptr<Sphere> Sphere::Create() {
	std::unique_ptr<Sphere> object = std::make_unique<Sphere>();
	object->Initialize();

	return object;
}

void Sphere::CreateRootSignature() {
	//DirectXCommonの取得
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

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
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
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
	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	LRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリをもとに生成
	hr = directXCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&sRootSignature_));
	assert(SUCCEEDED(hr));
}

void Sphere::CreatePSO() {
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//InputLayoutの設定
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//RasiterzerStatesの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏側を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderのコンパイル
	ComPtr<IDxcBlob> vertexShaderBlob = directXCommon->CompilerShader(L"Resources/Shaders/Object3D.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	ComPtr<IDxcBlob> pixelShaderBlob = directXCommon->CompilerShader(L"Resources/Shaders/Object3D.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeLineStateDesc{};
	graphicsPipeLineStateDesc.pRootSignature = sRootSignature_.Get();
	graphicsPipeLineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipeLineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	graphicsPipeLineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	graphicsPipeLineStateDesc.BlendState = blendDesc;
	graphicsPipeLineStateDesc.RasterizerState = rasterizerDesc;
	graphicsPipeLineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipeLineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//書き込むRTVの情報
	graphicsPipeLineStateDesc.NumRenderTargets = 1;
	graphicsPipeLineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用する形状タイプ
	graphicsPipeLineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むのかの設定
	graphicsPipeLineStateDesc.SampleDesc.Count = 1;
	graphicsPipeLineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	LRESULT hr = directXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipeLineStateDesc, IID_PPV_ARGS(&sGraphicsPipelineState_));
	assert(SUCCEEDED(hr));
}

void Sphere::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//VertexResourceの生成
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * kVertexNumber);
	//MaterialResourceの生成
	materialResource_ = CreateBufferResource(sizeof(Vector4));
	//wvpResourceの生成
	wvpResource_ = CreateBufferResource(sizeof(Matrix4x4));

	//リソースの先頭のアドレスを使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexNumber;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//経度分割1つ分の角度
	const float pi = 3.14f;
	const float kLonEvery = pi * 2.0f / float(kSubDivision);
	//緯度分割1つ分の角度
	const float kLatEvery = pi / float(kSubDivision);
	//緯度の方向に分割
	for (UINT latIndex = 0; latIndex < kSubDivision; latIndex++) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		//経度の方向に分割
		for (UINT lonIndex = 0; lonIndex < kSubDivision; lonIndex++) {
			uint32_t start = (latIndex * kSubDivision + lonIndex) * 6;
			float lon = (lonIndex + kSubDivision / 4.0f) * kLonEvery;
			//a, b, c, dの場所
			Vector4 a, b, c, d;
			a = { std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon),
				1.0f};

			b = { std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon),
				1.0f };

			c = { std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery),
				1.0f };

			d = { std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery),
				1.0f };

			//隙間をなくすための処理
			if (lonIndex == kSubDivision - 1) {
				lon = kSubDivision / 4.0f * kLonEvery;

				c = { std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon),
				1.0f };

				d = { std::cos(lat + kLatEvery) * std::cos(lon),
					std::sin(lat + kLatEvery),
					std::cos(lat + kLatEvery) * std::sin(lon),
					1.0f };
			}

			//頂点データの入力
			Vector2 leftDownUV = { float(lonIndex) / float(kSubDivision), 1.0f - float(latIndex) / float(kSubDivision) };
			Vector2 leftTopUV = { float(lonIndex) / float(kSubDivision), 1.0f - float(latIndex + 1) / float(kSubDivision) };
			Vector2 rightDownUV = { float(lonIndex + 1) / float(kSubDivision), 1.0f - float(latIndex) / float(kSubDivision) };
			Vector2 rightTopUV = { float(lonIndex + 1) / float(kSubDivision), 1.0f - float(latIndex + 1) / float(kSubDivision) };
			vertexData_[start].position = a;
			vertexData_[start].texcoord = leftDownUV;
			vertexData_[start + 1].position = b;
			vertexData_[start + 1].texcoord = leftTopUV;
			vertexData_[start + 2].position = c;
			vertexData_[start + 2].texcoord = rightDownUV;
			vertexData_[start + 3].position = b;
			vertexData_[start + 3].texcoord = leftTopUV;
			vertexData_[start + 4].position = d;
			vertexData_[start + 4].texcoord = rightTopUV;
			vertexData_[start + 5].position = c;
			vertexData_[start + 5].texcoord = rightDownUV;
		}
	}

	//Materialデータの記入
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//色の書き込み
	*materialData_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	//wvpデータの記入
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	*wvpData_ = MakeIdentity4x4();

	//トランスフォームの初期化
	transform_.scale = { 1.0f, 1.0f, 1.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, 0.0f };
}

void Sphere::Update() {

	ImGui::Begin("Sphere");
	ImGui::SliderFloat3("translate", &transform_.translate.x, -10, 10);
	ImGui::SliderFloat3("rotate", &transform_.rotate.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &transform_.scale.x, -10, 10);
	ImGui::ColorEdit3("Color", &materialData_->x);
	ImGui::End();
}

void Sphere::Draw(Matrix4x4 viewProjectionMatrix, UINT textureName) {
	TextureManager* textureManager = TextureManager::GetInstance();

	//ワールドMatrixの更新
	worldMatrix_ = MakeAffineMatrix(transform_);
	//カメラ移動によるwvpの変化
	*wvpData_ = Multiply(worldMatrix_, viewProjectionMatrix);

	//VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//マテリアルCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	sCommandList_->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureName));
	//描画
	sCommandList_->DrawInstanced(kVertexNumber, 1, 0, 0);
}