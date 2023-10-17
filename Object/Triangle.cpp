#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

/// <summary>
/// 静的メンバ変数の実体化
/// </summary>
ComPtr<ID3D12RootSignature> Triangle::sRootSignature_;
ComPtr<ID3D12PipelineState> Triangle::sGraphicsPipelineState_;
D3D12_VIEWPORT Triangle::sViewPort_;
D3D12_RECT Triangle::sScissorRect_;
ID3D12GraphicsCommandList* Triangle::sCommandList_ = nullptr;

void Triangle::StaticInitialize() {
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

void Triangle::PreDraw(ID3D12GraphicsCommandList* commandList) {
	//1フレーム前にPostDrawが呼ばれていなかったらエラー
	assert(Triangle::sCommandList_ == nullptr);
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

void Triangle::PostDraw() {
	//コマンドリストを解除
	sCommandList_ = nullptr;
}

std::unique_ptr<Triangle> Triangle::Create() {
	std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>();
	triangle->Initialize();

	return triangle;
}

void Triangle::CreateRootSignature() {
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

void Triangle::CreatePSO() {
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

	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeLineStateDesc{};
	graphicsPipeLineStateDesc.pRootSignature = sRootSignature_.Get();
	graphicsPipeLineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipeLineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	graphicsPipeLineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	graphicsPipeLineStateDesc.BlendState = blendDesc;
	graphicsPipeLineStateDesc.RasterizerState = rasterizerDesc;
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

void Triangle::Initialize() {

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
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = {-0.5f, -0.5f, 0.0f, 1.0f};
	vertexData[0].texcoord = {0.0f, 1.0f};
	//上
	vertexData[1].position = {0.0f, 0.5f, 0.0f, 1.0f};
	vertexData[1].texcoord = {0.5f, 0.0f};
	//右下
	vertexData[2].position = {0.5f, -0.5f, 0.0f, 1.0f};
	vertexData[2].texcoord = {1.0f, 1.0f};

	//Materialデータの記入
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//色の書き込み
	*materialData_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	//wvpデータの記入
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	*wvpData_ = MakeIdentity4x4();

	//トランスフォームの初期化
	transform_.scale = { 1.0f, 1.0f, 1.0f };
	transform_.rotate = { 0.0f, 0.0f, 0.0f };
	transform_.translate = { 0.0f, 0.0f, 0.0f };

}

void Triangle::Update() {
	transform_.rotate.y += 0.03f;

	//ワールドMatrixの更新
	worldMatrix_ = MakeAffineMatrix(transform_);
}

void Triangle::Draw(Matrix4x4 viewProjectionMatrix) {
	TextureManager* textureManager = TextureManager::GetInstance();

	//カメラ移動によるwvpの変化
	Matrix4x4 wvpMatrix = Multiply(worldMatrix_, viewProjectionMatrix);
	*wvpData_ = wvpMatrix;

	//VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//マテリアルCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	sCommandList_->SetGraphicsRootDescriptorTable(2, textureManager->textureSrvHandleGPU_[UVCHECKER]);
	//描画
	sCommandList_->DrawInstanced(kVertexNumber, 1, 0, 0);
}