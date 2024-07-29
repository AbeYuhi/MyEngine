#include "WireFrameSphere.h"

WireFrameSphere::WireFrameSphere()
{
}

WireFrameSphere::~WireFrameSphere()
{
}

std::shared_ptr<WireFrameSphere> WireFrameSphere::sWireFrameSphere_;
std::shared_ptr<WireFrameSphere> WireFrameSphere::Create() {

	if (!sWireFrameSphere_) {
		sWireFrameSphere_ = std::make_shared<WireFrameSphere>();
		sWireFrameSphere_->Initialize();
	}

	return sWireFrameSphere_;
}

void WireFrameSphere::Initialize() {
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

	const uint32_t kSubdivision = 16;
	const float kLonEvery = (2 * 3.14f) / kSubdivision;
	const float kLatEvery = (3.14f) / kSubdivision;
	int index = 0;
	//緯度の方向に分割 -π/1 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -3.14f / 2.0f + kLatEvery * latIndex;
		//経度の方向に分割 0～2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			//world座標系でのa, b, cを求める
			Vector3 a, b, c;
			a = { std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon) };

			b = { std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon) };

			c = { std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery) };

			vertexData_[index * 3].position = { a.x, a.y, a.z, 1.0f };
			vertexData_[index * 3].texcoord = { 0.0f, 1.0f };
			vertexData_[index * 3].normal = { 0.0f, 0.0f, 1.0f };
			vertexData_[index * 3 + 1].position = { b.x, b.y, b.z, 1.0f };
			vertexData_[index * 3 + 1].texcoord = { 0.0f, 1.0f };
			vertexData_[index * 3 + 1].normal = { 0.0f, 0.0f, 1.0f };
			vertexData_[index * 3 + 2].position = { c.x, c.y, c.z, 1.0f };
			vertexData_[index * 3 + 2].texcoord = { 0.0f, 1.0f };
			vertexData_[index * 3 + 2].normal = { 0.0f, 0.0f, 1.0f };
			index++;
		}
	}

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexNumber;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	index = 0;
	for (int i = 0; i < 16 * 16; i++) {
		uint32_t first = i * 3;
		indexData_[index++] = first;
		indexData_[index++] = first + 1;
		indexData_[index++] = first;
		indexData_[index++] = first + 2;
	}
}

void WireFrameSphere::Draw(RenderItem& renderItem) {
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

	renderItem.Update();

	//ViewPortの設定
	dxCommon->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	dxCommon->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO(PipelineState::kWireFrame));
	//ルートシグネチャの設定
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature(PipelineState::kWireFrame));
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