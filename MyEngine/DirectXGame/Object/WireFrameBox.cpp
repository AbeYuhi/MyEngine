#include "WireFrameBox.h"

WireFrameBox::WireFrameBox()
{
}

WireFrameBox::~WireFrameBox()
{
}

std::shared_ptr<WireFrameBox> WireFrameBox::sWireFrameBox_;
std::shared_ptr<WireFrameBox> WireFrameBox::Create() {

	if (!sWireFrameBox_) {
		sWireFrameBox_ = std::make_shared<WireFrameBox>();
		sWireFrameBox_->Initialize();
	}

	return sWireFrameBox_;
}

void WireFrameBox::Initialize() {
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

	//奥
	vertexData_[0].position = { 0.5f, -0.5f, 0.5f, 1.0f }; //右下
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[0].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[1].position = { 0.5f, 0.5f, 0.5f, 1.0f }; //右上
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[2].position = { -0.5f, -0.5f, 0.5f, 1.0f }; //左下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[2].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[3].position = { -0.5f, 0.5f, 0.5f, 1.0f }; //左上
	vertexData_[3].texcoord = { 1.0f, 0.0f };
	vertexData_[3].normal = { 0.0f, 0.0f, 1.0f };

	//手前
	vertexData_[4].position = { 0.5f, -0.5f, -0.5f, 1.0f }; //右下
	vertexData_[4].texcoord = { 0.0f, 1.0f };
	vertexData_[4].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[5].position = { 0.5f, 0.5f, -0.5f, 1.0f }; //右上
	vertexData_[5].texcoord = { 0.0f, 0.0f };
	vertexData_[5].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[6].position = { -0.5f, -0.5f, -0.5f, 1.0f }; //左下
	vertexData_[6].texcoord = { 1.0f, 1.0f };
	vertexData_[6].normal = { 0.0f, 0.0f, -1.0f };
	vertexData_[7].position = { -0.5f, 0.5f, -0.5f, 1.0f }; //左上
	vertexData_[7].texcoord = { 1.0f, 0.0f };
	vertexData_[7].normal = { 0.0f, 0.0f, -1.0f };

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexNumber;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	//奥
	indexData_[0] = 0;	indexData_[1] = 1;	
	indexData_[2] = 2;	indexData_[3] = 3;
	indexData_[4] = 2;	indexData_[5] = 0;
	indexData_[6] = 3;	indexData_[7] = 1;	
	//手前
	indexData_[8] = 4;	indexData_[9] = 5;
	indexData_[10] = 6;	indexData_[11] = 7;
	indexData_[12] = 5;	indexData_[13] = 7;
	indexData_[14] = 4;	indexData_[15] = 6;
	//側面
	indexData_[16] = 0;	indexData_[17] = 4;
	indexData_[18] = 1;	indexData_[19] = 5;
	indexData_[20] = 2;	indexData_[21] = 6;
	indexData_[22] = 3;	indexData_[23] = 7;
}

void WireFrameBox::Draw(RenderItem& renderItem) {
	TextureManager* textureManager = TextureManager::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//非表示なら表示しない
	if (renderItem.materialInfo_.isInvisible_) {
		return;
	}
#ifdef NDEBUG
	return;
#endif // NDEBUG


	//ViewPortの設定
	dxCommon->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	dxCommon->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO(PipelineState::kWireFrame));
	//ルートシグネチャの設定
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature());
	//プリミティブ形状を設定
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//VBVの設定
	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	dxCommon->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//マテリアルCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem.materialInfo_.resource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.worldTransform_.resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(1));
	//描画
	dxCommon->GetCommandList()->DrawIndexedInstanced(kIndexNumber, 1, 0, 0, 0);
}