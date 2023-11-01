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
Vector3 Triangle::sDefaultPos[3] = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}};

std::unique_ptr<Triangle> Triangle::Create(Vector3 pos[3]) {
	std::unique_ptr<Triangle> object = std::make_unique<Triangle>();
	object->Initialize(pos);

	return object;
}

void Triangle::Initialize(Vector3 pos[3]) {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//VertexResourceの生成
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * kVertexNumber);

	//リソースの先頭のアドレスを使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexNumber;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//左下
	vertexData_[0].position = { pos[0].x, pos[0].y, pos[0].z, 1.0f };
	vertexData_[0].texcoord = {0.0f, 1.0f};
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
	//上
	vertexData_[1].position = { pos[1].x, pos[1].y, pos[1].z, 1.0f};
	vertexData_[1].texcoord = {0.5f, 0.0f};
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
	//右下
	vertexData_[2].position = { pos[2].x, pos[2].y, pos[2].z, 1.0f};
	vertexData_[2].texcoord = {1.0f, 1.0f};
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
}

void Triangle::Draw(RenderItem& renderItem, UINT textureName) {
	TextureManager* textureManager = TextureManager::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//ViewPortの設定
	dxCommon->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	dxCommon->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO());
	//ルートシグネチャの設定
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature());
	//プリミティブ形状を設定
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//VBVの設定
	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//マテリアルCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem.materialInfo_.resource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.worldTransform_.resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureName));
	//描画
	dxCommon->GetCommandList()->DrawInstanced(kVertexNumber, 1, 0, 0);
}