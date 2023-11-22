#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

std::unique_ptr<Sprite> Sprite::Create(Vector2 spriteSize, uint32_t textureHandle, Vector2 anchorPoint, Vector2 baseUVPos, bool isFlipX, bool isFlipY) {
	std::unique_ptr<Sprite> object = std::make_unique<Sprite>();
	object->Initialize(spriteSize, textureHandle, anchorPoint, baseUVPos, isFlipX, isFlipY);
	return object;
}

void Sprite::Initialize(Vector2 spriteSize, uint32_t textureHandle, Vector2 anchorPoint, Vector2 baseUVPos, bool isFlipX, bool isFlipY) {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//メンバ変数の初期化
	//size
	size_ = spriteSize;
	//アンカーポイント
	anchorPoint_ = anchorPoint;
	//BaseUVPos
	baseUvPos_ = baseUVPos;
	//反転
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;
	//非表示か
	isInvisible_ = false;

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
	vertexData_[0].position = { 0.0f, size_.y, 0.0f, 1.0f }; //左下
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[0].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f }; //左上
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[2].position = { size_.x, size_.y, 0.0f, 1.0f }; //右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[2].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[3].position = { size_.x, 0.0f, 0.0f, 1.0f }; //右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
	vertexData_[3].normal = { 0.0f, 0.0f, -1.0f };

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexNumber;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0;	indexData_[1] = 1;	indexData_[2] = 2;
	indexData_[3] = 1;	indexData_[4] = 3;	indexData_[5] = 2;

	TransferVertices();
}

void Sprite::TransferVertices() {

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bot = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	if (isFlipY_) {
		top = -top;
		bot = -bot;
	}

	vertexData_[0].position = { left, bot, 0.0f, 1.0f }; //左下
	vertexData_[1].position = { left, top, 0.0f, 1.0f }; //左上
	vertexData_[2].position = { right, bot, 0.0f, 1.0f }; //右下
	vertexData_[3].position = { right, top, 0.0f, 1.0f }; //右上

	float uvLeft = (0.0f - baseUvPos_.x);
	float uvRight = (1.0f - baseUvPos_.x);
	float uvTop = (0.0f - baseUvPos_.y);
	float uvBot = (1.0f - baseUvPos_.y);

	vertexData_[0].texcoord = { uvLeft, uvBot, }; //左下
	vertexData_[1].texcoord = { uvLeft, uvTop, }; //左上
	vertexData_[2].texcoord = { uvRight, uvBot }; //右下
	vertexData_[3].texcoord = { uvRight, uvTop }; //右上
}

void Sprite::SetTextureHandle(uint32_t textureHandle) {
	textureHandle_ = textureHandle;
	TransferVertices();
}

void Sprite::SetSize(Vector2 size) {
	size_ = size; 
	TransferVertices();
}

void Sprite::SetAnchorPoint(Vector2 anchorPoint) { 
	anchorPoint_ = anchorPoint; 
	TransferVertices();
}

void Sprite::SetBaseUvPos(Vector2 baseUvPos) {
	baseUvPos_ = baseUvPos;
	TransferVertices();
}

void Sprite::SetIsFlipX(bool isFlip) {
	isFlipX_ = isFlip; 
	TransferVertices();
}

void Sprite::SetIsFlipY(bool isFlip) { 
	isFlipY_ = isFlip;
	TransferVertices();
}

void Sprite::Draw(RenderItem& renderItem) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//非表示なら表示しない
	if (isInvisible_) {
		return;
	}

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
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem.materialInfo_.resource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.worldTransform_.resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureHandle_));
	//描画
	dxCommon->GetCommandList()->DrawIndexedInstanced(kIndexNumber, 1, 0, 0, 0);
}

void Sprite::Draw(ParticleDrawInfo drawInfo) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

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
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureHandle_));
	//描画
	dxCommon->GetCommandList()->DrawIndexedInstanced(kIndexNumber, (UINT)*drawInfo.kMaxParticleCount_, 0, 0, 0);
}