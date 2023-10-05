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

void Triangle::CreateRootSignature() {
	//DirectXCommonの取得
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();

	//RootSignature生成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
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
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
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
	IDxcBlob* vertexShaderBlob = directXCommon->CompilerShader(L"Resources/Shaders/Object3D.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	IDxcBlob* pixelShaderBlob = directXCommon->CompilerShader(L"Resources/Shaders/Object3D.PS.hlsl", L"ps_6_0");
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
	CreateVertexResource();

	//リソースの先頭のアドレスを使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * kVertexNumber;
	vertexBufferView_.StrideInBytes = sizeof(Vector4);

	//Resourceにデータを書き込む
	Vector4* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
	//上
	vertexData[1] = {0.0f, 0.5f, 0.0f, 1.0f};
	//右下
	vertexData[2] = {0.5f, -0.5f, 0.0f, 1.0f};
}

void Triangle::Update() {

}

void Triangle::Draw() {
	//VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//描画
	sCommandList_->DrawInstanced(kVertexNumber, 1, 0, 0);
}

void Triangle::CreateVertexResource() {
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//バックバッファリソース
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vector4) * kVertexNumber; //リソースのサイズ
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に生成
	LRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource_));
	assert(SUCCEEDED(hr));
}