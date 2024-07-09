#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

std::shared_ptr<SkyBox> SkyBox::Create(std::string filePath) {

	std::shared_ptr<SkyBox> skybox = std::make_shared<SkyBox>();
	skybox->Initialize(filePath);

	return skybox;
}

void SkyBox::Initialize(std::string filePath) {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//テクスチャの読み込み
	textureHandle_ = TextureManager::GetInstance()->Load(filePath);

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

	//右面
	vertexData_[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[1].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[2].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[3].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	//左面
	vertexData_[4].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[7].position = { -1.0f, -1.0f, 1.0f, 1.0f };

	//後面
	vertexData_[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[10].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[11].position = { 1.0f, -1.0f, 1.0f, 1.0f };

	//前面
	vertexData_[12].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[13].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };

	//上面
	vertexData_[16].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[17].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[18].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[19].position = { 1.0f, 1.0f, 1.0f, 1.0f };

	//下面
	vertexData_[20].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[21].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	for (int i = 0; i < kVertexNumber; i++) {
		vertexData_[i].texcoord = { 0.0f, 1.0f };
		vertexData_[i].normal = { 0.0f, 0.0f, 1.0f };
	}

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexNumber;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	//右面
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 2; indexData_[4] = 1; indexData_[5] = 3;
	//左面
	indexData_[6] = 4; indexData_[7] = 5; indexData_[8] = 6;
	indexData_[9] = 6; indexData_[10] = 5; indexData_[11] = 7;
	//後面
	indexData_[12] = 8;	indexData_[13] = 9; indexData_[14] = 10;	
	indexData_[15] = 10; indexData_[16] = 9; indexData_[17] = 11;
	//前面
	indexData_[18] = 12; indexData_[19] = 13; indexData_[20] = 14;
	indexData_[21] = 14; indexData_[22] = 13; indexData_[23] = 15;
	//上面
	indexData_[24] = 16; indexData_[25] = 17; indexData_[26] = 18;
	indexData_[27] = 18; indexData_[28] = 17; indexData_[29] = 19;
	//下面
	indexData_[30] = 20; indexData_[31] = 21; indexData_[32] = 22;
	indexData_[33] = 22; indexData_[34] = 21; indexData_[35] = 23;
}

void SkyBox::Draw(RenderItem& renderItem) {
	TextureManager* textureManager = TextureManager::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//非表示なら表示しない
	if (renderItem.materialInfo_.isInvisible_) {
		return;
	}

	renderItem.Update();

	//ViewPortの設定
	dxCommon->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	dxCommon->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO(PipelineState::kSkyBox));
	//ルートシグネチャの設定
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature(PipelineState::kSkyBox));
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