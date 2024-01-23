#include "Model.h"

Model::Model() {}
Model::~Model() {}

std::unique_ptr<Model> Model::Create(const std::string filename) {
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
	for (auto& mesh : meshs_) {
		mesh.vertexResource = CreateBufferResource(sizeof(VertexData) * mesh.modelData.vertices.size());

		//頂点バッファビューを作成
		mesh.vertexBufferView.BufferLocation = mesh.vertexResource->GetGPUVirtualAddress();
		mesh.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * mesh.modelData.vertices.size());
		mesh.vertexBufferView.StrideInBytes = sizeof(VertexData);

		//頂点リソースにデータを書き込む
		mesh.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&mesh.vertexData));
		std::memcpy(mesh.vertexData, mesh.modelData.vertices.data(), sizeof(VertexData) * mesh.modelData.vertices.size());
	}
}

void Model::Draw(RenderItem& renderItem) {
	//シングルトーンの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	if (renderItem.materialInfo_.isInvisible_) {
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
	for (auto& mesh : meshs_) {
		//VBVの設定
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		//マテリアルCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem.materialInfo_.resource_->GetGPUVirtualAddress());
		//wvpCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.worldTransform_.resource_->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(mesh.textureHandle));
		//描画
		dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), 1, 0, 0);
	}
}

void Model::Draw(RenderItem& renderItem, uint32_t textureHandle) {
	//シングルトーンの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	if (renderItem.materialInfo_.isInvisible_) {
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
	for (auto& mesh : meshs_) {
		//VBVの設定
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		//マテリアルCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem.materialInfo_.resource_->GetGPUVirtualAddress());
		//wvpCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.worldTransform_.resource_->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureHandle));
		//描画
		dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), 1, 0, 0);
	}
}

void Model::Draw(ParticleDrawInfo drawInfo) {
	//シングルトーンの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	if (drawInfo.materialInfo_->isInvisible_) {
		return;
	}

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
	for (auto& mesh : meshs_) {
		//VBVの設定
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		//マテリアルCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, drawInfo.materialInfo_->resource_->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, drawInfo.srvHandle_->GPUHandle);
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(mesh.textureHandle));
		//描画
		dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), (UINT)*drawInfo.kMaxParticleCount_, 0, 0);
	}
}

void Model::Draw(ParticleDrawInfo drawInfo, uint32_t textureHandle) {
	//シングルトーンの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	TextureManager* textureManager = TextureManager::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();

	if (drawInfo.materialInfo_->isInvisible_) {
		return;
	}

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
	for (auto& mesh : meshs_) {
		//VBVの設定
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		//マテリアルCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, drawInfo.materialInfo_->resource_->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, drawInfo.srvHandle_->GPUHandle);
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureHandle));
		//描画
		dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), (UINT)*drawInfo.kMaxParticleCount_, 0, 0);
	}
}

void Model::LoadObjFile(const std::string& filename) {

	//assimpで読み込む
	Assimp::Importer importer;
	std::string filePath = "Resources/Images/" + filename + "/" + filename + ".obj";
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());
	
	//meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
		Mesh modelPart = {};
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		//中身の解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			//Vertexの解析
			for (uint32_t element = 0; element < face.mNumIndices; element++) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;

				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };

				vertex.position.z *= -1.0f;
				vertex.normal.z *= -1.0f;
				modelPart.modelData.vertices.push_back(vertex);
			}
		}

		//マテリアルのインプット
		unsigned int materialIndex = mesh->mMaterialIndex;
		const aiMaterial* material = scene->mMaterials[materialIndex];
		aiString textureFilePath;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
		modelPart.modelData.material.textureFilePath = filename + "/" + textureFilePath.C_Str();
		std::string textureName = textureFilePath.C_Str();

		//もしテクスチャが見つからなかった場合は白い画像を入れる
		if (!modelPart.modelData.material.textureFilePath.empty()) {
			modelPart.textureHandle = TextureManager::Load(textureName, modelPart.modelData.material.textureFilePath);
		}
		else {
			modelPart.textureHandle = TextureManager::Load("whiteTexture2x2.png");
		}

		meshs_.push_back(modelPart);
	}
}