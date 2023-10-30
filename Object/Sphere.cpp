#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

/// <summary>
/// 静的メンバ変数の実体化
/// </summary>
ID3D12GraphicsCommandList* Sphere::sCommandList_ = nullptr;

void Sphere::PreDraw(ID3D12GraphicsCommandList* commandList) {
	//1フレーム前にPostDrawが呼ばれていなかったらエラー
	assert(Sphere::sCommandList_ == nullptr);
	//コマンドリストのセット
	sCommandList_ = commandList;

	//PSOManagerの取得
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//ViewPortの設定
	sCommandList_->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	sCommandList_->RSSetScissorRects(1, psoManager->GetScissorRect());
	//パイプラインステートの設定
	sCommandList_->SetPipelineState(psoManager->GetPSO());
	//ルートシグネチャの設定
	sCommandList_->SetGraphicsRootSignature(psoManager->GetRootSignature());
	//プリミティブ形状を設定
	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sphere::PostDraw() {
	//コマンドリストを解除
	sCommandList_ = nullptr;
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
	//MaterialResourceの生成
	materialResource_ = CreateBufferResource(sizeof(Material));
	//transformationMatrixResourceの生成
	transformMatrixResource_ = CreateBufferResource(sizeof(TransformMatrix));

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

	//Materialデータの記入
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//色の書き込み
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLightint = true;
	materialData_->uvTransform = MakeIdentity4x4();

	//wvpデータの記入
	transformMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transform_.matrix_));
	transform_.matrix_->World_ = MakeIdentity4x4();

	//トランスフォームの初期化
	transform_.data_.scale_ = { 1.0f, 1.0f, 1.0f };
	transform_.data_.rotate_ = { 0.0f, 0.0f, 0.0f };
	transform_.data_.translate_ = { 0.0f, 0.0f, 0.0f };
}

void Sphere::Update() {

	ImGui::Begin("Sphere");
	ImGui::SliderFloat3("translate", &transform_.data_.translate_.x, -10, 10);
	ImGui::SliderFloat3("rotate", &transform_.data_.rotate_.x, -2.0f * 3.14f, 2.0f * 3.14f);
	ImGui::SliderFloat3("scale", &transform_.data_.scale_.x, -10, 10);
	ImGui::ColorEdit3("Color", &materialData_->color.x);
	ImGui::End();
}

void Sphere::Draw(Matrix4x4 viewProjectionMatrix, UINT textureName) {
	TextureManager* textureManager = TextureManager::GetInstance();

	//ワールドMatrixの更新
	transform_.UpdateWorld();
	//カメラ移動によるwvpの変化
	transform_.UpdateWVP(viewProjectionMatrix);

	//VBVの設定
	sCommandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//マテリアルCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	sCommandList_->SetGraphicsRootConstantBufferView(1, transformMatrixResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	sCommandList_->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureName));
	//描画
	sCommandList_->DrawInstanced(kVertexNumber, 1, 0, 0);
}