#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

std::unique_ptr<Sphere> Sphere::Create() {
	std::unique_ptr<Sphere> object = std::make_unique<Sphere>();
	object->Initialize();

	return object;
}

void Sphere::Initialize() {
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
	//経度分割1つ分の角度
	const float pi = 3.14f;
	const float kLonEvery = pi * 2.0f / float(kSubDivision);
	//緯度分割1つ分の角度
	const float kLatEvery = pi / float(kSubDivision);
	//緯度の方向に分割
	for (UINT latIndex = 0; latIndex < kSubDivision; latIndex++) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;
		//経度の方向に分割
		for (UINT lonIndex = 0; lonIndex < kSubDivision; lonIndex++) {
			uint32_t start = (latIndex * kSubDivision + lonIndex) * 6;
			float lon = (lonIndex + kSubDivision / 4.0f) * kLonEvery;
			//a, b, c, dの場所
			Vector4 a, b, c, d;
			a = { std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon),
				1.0f};

			b = { std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon),
				1.0f };

			c = { std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery),
				1.0f };

			d = { std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery),
				1.0f };

			//隙間をなくすための処理
			if (lonIndex == kSubDivision - 1) {
				lon = kSubDivision / 4.0f * kLonEvery;

				c = { std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon),
				1.0f };

				d = { std::cos(lat + kLatEvery) * std::cos(lon),
					std::sin(lat + kLatEvery),
					std::cos(lat + kLatEvery) * std::sin(lon),
					1.0f };
			}

			//頂点データの入力
			Vector2 leftDownUV = { float(lonIndex) / float(kSubDivision), 1.0f - float(latIndex) / float(kSubDivision) };
			Vector2 leftTopUV = { float(lonIndex) / float(kSubDivision), 1.0f - float(latIndex + 1) / float(kSubDivision) };
			Vector2 rightDownUV = { float(lonIndex + 1) / float(kSubDivision), 1.0f - float(latIndex) / float(kSubDivision) };
			Vector2 rightTopUV = { float(lonIndex + 1) / float(kSubDivision), 1.0f - float(latIndex + 1) / float(kSubDivision) };
			vertexData_[start].position = a;
			vertexData_[start].texcoord = leftDownUV;
			vertexData_[start].normal = { vertexData_[start].position.x, vertexData_[start].position.y, vertexData_[start].position.z };
			vertexData_[start + 1].position = b;
			vertexData_[start + 1].texcoord = leftTopUV;
			vertexData_[start + 1].normal = { vertexData_[start + 1].position.x, vertexData_[start + 1].position.y, vertexData_[start + 1].position.z };
			vertexData_[start + 2].position = c;
			vertexData_[start + 2].texcoord = rightDownUV;
			vertexData_[start + 2].normal = { vertexData_[start + 2].position.x, vertexData_[start + 2].position.y, vertexData_[start + 2].position.z };
			vertexData_[start + 3].position = b;
			vertexData_[start + 3].texcoord = leftTopUV;
			vertexData_[start + 3].normal = { vertexData_[start + 3].position.x, vertexData_[start + 3].position.y, vertexData_[start + 3].position.z };
			vertexData_[start + 4].position = d;
			vertexData_[start + 4].texcoord = rightTopUV;
			vertexData_[start + 4].normal = { vertexData_[start + 4].position.x, vertexData_[start + 4].position.y, vertexData_[start + 4].position.z };
			vertexData_[start + 5].position = c;
			vertexData_[start + 5].texcoord = rightDownUV;
			vertexData_[start + 5].normal = { vertexData_[start + 5].position.x, vertexData_[start + 5].position.y, vertexData_[start + 5].position.z };
		}
	}
}

void Sphere::Draw(RenderItem& renderItem, std::string textureName) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
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