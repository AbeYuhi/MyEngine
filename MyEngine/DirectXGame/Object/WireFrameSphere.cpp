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

	int vertexIndex = 0;
	for (int lat = 1; lat < kLatitudeCount; ++lat) {
		float theta = lat * DirectX::XM_PI / kLatitudeCount;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int lon = 0; lon <= kLongitudeCount; ++lon) {
			float phi = lon * 2.0f * DirectX::XM_PI / kLongitudeCount;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			vertexData_[vertexIndex].position.x = sinTheta * cosPhi;
			vertexData_[vertexIndex].position.y = cosTheta;
			vertexData_[vertexIndex].position.z = sinTheta * sinPhi;
			vertexIndex++;
		}
	}

	// Add poles
	vertexData_[vertexIndex].position = { 0.0f, 1.0f, 0.0f };
	vertexIndex++;
	vertexData_[vertexIndex].position = { 0.0f, -1.0f, 0.0f };

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * kIndexNumber;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	// Generate indices
	int indexIndex = 0;
	for (int lat = 0; lat < kLatitudeCount - 2; ++lat) {
		for (int lon = 0; lon < kLongitudeCount; ++lon) {
			int current = lat * (kLongitudeCount + 1) + lon;
			int next = current + kLongitudeCount + 1;

			indexData_[indexIndex++] = current;
			indexData_[indexIndex++] = current + 1;

			indexData_[indexIndex++] = current;
			indexData_[indexIndex++] = next;
		}
	}

	/*for (int lon = 0; lon < kLongitudeCount; ++lon) {
		int top = (kLatitudeCount - 2) * (kLongitudeCount + 1);
		int bottom = top + 1;
		int current = lon;
		int next = current + kLongitudeCount + 1;

		indexData_[indexIndex++] = top;
		indexData_[indexIndex++] = current;

		indexData_[indexIndex++] = bottom;
		indexData_[indexIndex++] = next;
	}*/
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