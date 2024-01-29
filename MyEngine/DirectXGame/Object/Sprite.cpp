#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

std::shared_ptr<Sprite> Sprite::Create() {
	std::shared_ptr<Sprite> object = std::make_shared<Sprite>();
	object->Initialize();
	return object;
}

void Sprite::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//VertexResourceの生成
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * kVertexNumber);
	//IndexResourceの生成
	indexResource_ = CreateBufferResource(sizeof(uint32_t) * kIndexNumber);

	//リソースの先頭のアドレスを使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexNumber;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//1枚目の三角形
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexNumber;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0;	indexData_[1] = 1;	indexData_[2] = 2;
	indexData_[3] = 1;	indexData_[4] = 3;	indexData_[5] = 2;
}

void Sprite::TransferVertices(SpriteData spriteData) {

	float left = (0.0f - spriteData.anchorPoint_.x) * spriteData.size_.x;
	float right = (1.0f - spriteData.anchorPoint_.x) * spriteData.size_.x;
	float top = (0.0f - spriteData.anchorPoint_.y) * spriteData.size_.y;
	float bot = (1.0f - spriteData.anchorPoint_.y) * spriteData.size_.y;

	if (spriteData.isFlipX_) {
		left = -left;
		right = -right;
	}

	if (spriteData.isFlipY_) {
		top = -top;
		bot = -bot;
	}

	vertexData_[0].position = { left, bot, 0.0f, 1.0f }; //左下
	vertexData_[1].position = { left, top, 0.0f, 1.0f }; //左上
	vertexData_[2].position = { right, bot, 0.0f, 1.0f }; //右下
	vertexData_[3].position = { right, top, 0.0f, 1.0f }; //右上

	/*float uvLeft = (0.0f - baseUvPos_.x);
	float uvRight = (1.0f - baseUvPos_.x);
	float uvTop = (0.0f - baseUvPos_.y);
	float uvBot = (1.0f - baseUvPos_.y);*/

	float uvLeft = spriteData.baseUvPos_.x;
	float uvRight = spriteData.baseUvPos_.x + spriteData.texSize_.x;
	float uvTop = spriteData.baseUvPos_.y;
	float uvBot = spriteData.baseUvPos_.y + spriteData.texSize_.y;

	vertexData_[0].texcoord = { uvLeft, uvBot, }; //左下
	vertexData_[1].texcoord = { uvLeft, uvTop, }; //左上
	vertexData_[2].texcoord = { uvRight, uvBot }; //右下
	vertexData_[3].texcoord = { uvRight, uvTop }; //右上
}

void Sprite::Draw(SpriteItem& spriteItem) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//非表示なら表示しない
	if (spriteItem.materialInfo_.isInvisible_) {
		return;
	}
	TransferVertices(spriteItem.spriteData_);

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
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, spriteItem.materialInfo_.resource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, spriteItem.worldTransform_.resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(spriteItem.spriteData_.textureHandle_));
	//描画
	dxCommon->GetCommandList()->DrawIndexedInstanced(kIndexNumber, 1, 0, 0, 0);
}

void Sprite::Draw(ParticleDrawInfo drawInfo, SpriteData spriteData) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//非表示なら表示しない
	if (drawInfo.materialInfo_->isInvisible_) {
		return;
	}
	TransferVertices(spriteData);

	//ViewPortの設定
	dxCommon->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	dxCommon->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO(PipelineState::kParticle));
	//ルートシグネチャの設定
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature(PipelineState::kParticle));
	//プリミティブ形状を設定
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//VBVの設定
	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	dxCommon->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, drawInfo.materialInfo_->resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, drawInfo.srvHandle_->GPUHandle);
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(spriteData.textureHandle_));
	//描画
	dxCommon->GetCommandList()->DrawIndexedInstanced(kIndexNumber, (UINT)*drawInfo.kMaxParticleCount_, 0, 0, 0);
}