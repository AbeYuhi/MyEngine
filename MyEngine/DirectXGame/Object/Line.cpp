#include "Line.h"

LineObj::LineObj()
{
}

LineObj::~LineObj()
{
}

std::shared_ptr<LineObj> LineObj::Create() {
	std::shared_ptr<LineObj> object = std::make_shared<LineObj>();
	object->Initialize();
	return object;
}

void LineObj::Initialize() {
	//dxCommonのインスタンスの取得
	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//VertexResourceの生成
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * kVertexNumber);

	//リソースの先頭のアドレスを使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexNumber;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//奥
	vertexData_[0].position = { 0.5f, -0.5f, 0.5f, 1.0f };
	vertexData_[0].texcoord = { 0.0f, 1.0f };
	vertexData_[0].normal = { 0.0f, 0.0f, 1.0f };
	vertexData_[1].position = { 0.5f, 0.5f, 0.5f, 1.0f };
	vertexData_[1].texcoord = { 0.0f, 0.0f };
	vertexData_[1].normal = { 0.0f, 0.0f, 1.0f };

	//
	renderItem_.Initialize();
}

void LineObj::Draw(Vector3 posA, Vector3 posB) {
	TextureManager* textureManager = TextureManager::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//非表示なら表示しない
	if (renderItem_.materialInfo_.isInvisible_) {
		return;
	}
#ifdef NDEBUG
	return;
#endif // NDEBUG

	//線の始点終点の設定
	vertexData_[0].position.x = posA.x;
	vertexData_[0].position.y = posA.y;
	vertexData_[0].position.z = posA.z;
	vertexData_[1].position.x = posB.x;
	vertexData_[1].position.y = posB.y;
	vertexData_[1].position.z = posB.z;


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
	//マテリアルCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem_.materialInfo_.resource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem_.worldTransform_.resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(1));
	//描画
	dxCommon->GetCommandList()->DrawInstanced(kVertexNumber, 1, 0, 0);
}