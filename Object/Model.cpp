#include "Model.h"

Model::Model(){}
Model::~Model() {}

/// <summary>
/// 静的メンバ変数の実体化
/// </summary>
UINT Model::sModelNum_ = 0;

std::unique_ptr<Model> Model::Create(const std::string filename) {
	sModelNum_++;
	std::unique_ptr<Model> object = std::make_unique<Model>();
	object->Initialize(filename);

	return object;
}

void Model::Initialize(const std::string filename) {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//モデル読み込み
	LoadObjFile(filename);

	//VertexResourceを作る
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	//MaterialDataResourceの生成
	materialResource_ = CreateBufferResource(sizeof(Material));
	
	//頂点バッファビューを作成
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	//MaterialDataデータの記入
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
	//色の書き込み
	material_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material_->enableLightint = true;
	material_->uvTransform = MakeIdentity4x4();

	uvTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	uvTransform_.rotate_ = { 0.0f, 0.0f, 0.0f };
	uvTransform_.translate_ = { 0.0f, 0.0f, 0.0f };
}

void Model::Draw(WorldTransform& transform) {
	//シングルトーンの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	//uvTransform
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
	//マテリアルCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvpCBufferの場所を設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transform.resource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	//描画
	dxCommon->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

void Model::LoadObjFile(const std::string& filename) {
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file("Resources/Images/" + filename + "/" + filename + ".obj");
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.z *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			//面は三角形限定
			VertexData triangle[3];
			for (int32_t faceVertex = 0; faceVertex < 3; faceVertex++) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; element++) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}
				//要素へのIndexから、実際の要素を取得して、頂点を構築
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				triangle[faceVertex] = {position, texcoord, normal};
			}
			modelData_.vertices.push_back(triangle[2]);
			modelData_.vertices.push_back(triangle[1]);
			modelData_.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;
			LoadMaterialDataTemplateFile(filename, materialFilename);
			TextureManager* textureManager = TextureManager::GetInstance();
			DirectXCommon* dxCommon = DirectXCommon::GetInstance();

			mipImages_ = textureManager->LoadTexture(modelData_.material.textureFilePath);

			//Metadata
			DirectX::TexMetadata metadata = {};
			metadata = mipImages_.GetMetadata();
			textureResources_ = textureManager->CreateTextureResource(metadata);
			ComPtr<ID3D12Resource> intermediateResource = textureManager->UploadTextureData(textureResources_.Get(), mipImages_);

			dxCommon->TransferCommandList();

			textureManager->CreateShaderResourceView(textureResources_.Get(), mipImages_, textureSrvHandleCPU_, textureSrvHandleGPU_, (TextureName::TEXTURENUM - 1) + sModelNum_);
		}
	}
}

void Model::LoadMaterialDataTemplateFile(const std::string& folderPath, const std::string& filename) {
	std::string line;
	std::ifstream file("Resources/Images/" + folderPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			modelData_.material.textureFilePath = folderPath + "/" + textureFilename;
		}
	}
}