#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

std::unique_ptr<Sprite> Sprite::Create(Vector2 spriteSize) {
	std::unique_ptr<Sprite> object = std::make_unique<Sprite>();
	object->Initialize(spriteSize);

	return object;
}

void Sprite::Initialize(Vector2 spriteSize) {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//VertexResourceの生成
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * kVertexNumber);
	//IndexResourceの生成
	indexResource_ = CreateBufferResource(sizeof(uint32_t) * kIndexNumber);

	//MaterialDataDataResourceの生成
	materialResource_ = CreateBufferResource(sizeof(Material));

	//リソースの先頭のアドレスを使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexNumber;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//1枚目の三角形
	vertexData_[0].position = { 0.0f, spriteSize.y, 0.0f, 1.0f }; //左下
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f }; //左上
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[2].position = { spriteSize.x, spriteSize.y, 0.0f, 1.0f }; //右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[3].position = { spriteSize.x, 0.0f, 0.0f, 1.0f }; //右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
	vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexNumber;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0;	indexData_[1] = 1;	indexData_[2] = 2;
	indexData_[3] = 1;	indexData_[4] = 3;	indexData_[5] = 2;

	//Materialデータの記入
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
	//色の書き込み
	material_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_->enableLightint = false;
	material_->uvTransform = MakeIdentity4x4();

	uvTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	uvTransform_.rotate_ = {0.0f, 0.0f, 0.0f};
	uvTransform_.translate_ = {0.0f, 0.0f, 0.0f};
}

void Sprite::Draw(WorldTransform& transform, UINT textureName) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	material_->uvTransform = MakeAffineMatrix(uvTransform_.scale_, uvTransform_.rotate_, uvTransform_.translate_);

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
	dxCommon->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transform.resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureName));
	//描画
	dxCommon->GetCommandList()->DrawIndexedInstanced(kIndexNumber, 1, 0, 0, 0);
}