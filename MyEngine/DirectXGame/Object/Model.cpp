#include "Model.h"

Model::Model() {}
Model::~Model() {}

std::map<std::string, std::shared_ptr<Model>> Model::sModels_;

std::shared_ptr<Model> Model::Create(const std::string& filepath, const std::string filename) {

	if (sModels_.find(filepath) == sModels_.end()) {
		sModels_[filepath] = std::make_shared<Model>();
		sModels_[filepath]->Initialize(filepath, filename);
	}
	
	return sModels_[filepath];
}

void Model::Initialize(const std::string& filepath, const std::string filename) {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//モデル読み込み
	LoadModelFile(filepath, filename);

	//Resourceの作成
	for (auto& mesh : meshs_) {

		//VertexResource
		mesh.vertexResource = CreateBufferResource(sizeof(VertexData) * mesh.modelData.vertices.size());
		//頂点バッファビューを作成
		mesh.vertexBufferView.BufferLocation = mesh.vertexResource->GetGPUVirtualAddress();
		mesh.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * mesh.modelData.vertices.size());
		mesh.vertexBufferView.StrideInBytes = sizeof(VertexData);
		//頂点リソースにデータを書き込む
		mesh.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&mesh.vertexData));
		std::memcpy(mesh.vertexData, mesh.modelData.vertices.data(), sizeof(VertexData) * mesh.modelData.vertices.size());

		//IndexResource	
		mesh.indexResource = CreateBufferResource(sizeof(IndexData) * mesh.modelData.indices.size());
		//頂点バッファビューを作成
		mesh.indexBufferView.BufferLocation = mesh.indexResource->GetGPUVirtualAddress();
		mesh.indexBufferView.SizeInBytes = static_cast<uint32_t>(sizeof(IndexData) * mesh.modelData.indices.size());
		mesh.indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		//頂点リソースにデータを書き込む
		mesh.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&mesh.indexData));
		std::memcpy(mesh.indexData, mesh.modelData.indices.data(), sizeof(IndexData) * mesh.modelData.indices.size());
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

	int meshIndex = 0;
	for (auto& mesh : meshs_) {
		if (isGltf_) {
			renderItem.UpdateGltf(mesh, meshIndex);
		}

		//VBVの設定
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		//IBVの設定
		dxCommon->GetCommandList()->IASetIndexBuffer(&mesh.indexBufferView);
		//マテリアルCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem.materialInfo_.resource_->GetGPUVirtualAddress());
		//wvpCBufferの場所を設定
		if (isGltf_) {
			dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.meshWorldTransforms_[meshIndex].resource_->GetGPUVirtualAddress());
		}
		else {
			dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.worldTransform_.resource_->GetGPUVirtualAddress());
		}
		//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(mesh.textureHandle));
		//描画
		dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), 1, 0, 0);
		//dxCommon->GetCommandList()->DrawIndexedInstanced(UINT(mesh.modelData.indices.size() * 3), 1, 0, 0, 0);
		meshIndex++;
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
	
	int meshIndex = 0;
	for (auto& mesh : meshs_) {
		if (isGltf_) {
			renderItem.UpdateGltf(mesh, meshIndex);
		}

		//VBVの設定
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		//IBVの設定
		dxCommon->GetCommandList()->IASetIndexBuffer(&mesh.indexBufferView);
		//マテリアルCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, renderItem.materialInfo_.resource_->GetGPUVirtualAddress());
		//wvpCBufferの場所を設定
		if (isGltf_) {
			dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.meshWorldTransforms_[meshIndex].resource_->GetGPUVirtualAddress());
		}
		else {
			dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, renderItem.worldTransform_.resource_->GetGPUVirtualAddress());
		}
		//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureHandle));
		//描画
		dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), 1, 0, 0);
		//dxCommon->GetCommandList()->DrawIndexedInstanced(UINT(mesh.modelData.indices.size() * 3), 1, 0, 0, 0);
		meshIndex++;
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
		//IBVの設定
		dxCommon->GetCommandList()->IASetIndexBuffer(&mesh.indexBufferView);
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
		//IBVの設定
		dxCommon->GetCommandList()->IASetIndexBuffer(&mesh.indexBufferView);
		//マテリアルCBufferの場所を設定
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, drawInfo.materialInfo_->resource_->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定、2はrootParameter[2]である
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, drawInfo.srvHandle_->GPUHandle);
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager->GetTextureHandleGPU(textureHandle));
		//描画
		dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), (UINT)*drawInfo.kMaxParticleCount_, 0, 0);
		//dxCommon->GetCommandList()->DrawIndexedInstanced(UINT(mesh.modelData.indices.size() * 3), (UINT)*drawInfo.kMaxParticleCount_, 0, 0, 0);
	}
}

void Model::LoadModelFile(const std::string& filepath, const std::string& filename) {

	//assimpで読み込む
	Assimp::Importer importer;
	std::string filePath = "Resources/Images/" + filepath + "/" + filename;
	filePath_ = filePath;
	const aiScene* scene = importer.ReadFile(filePath_.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());

	//拡張子の取得
	std::string extension = "";
	auto extPos = filename.find_last_of('.');
	if (extPos != std::string::npos) {
		extension = filename.substr(extPos + 1);
	}
	if (extension == "gltf") {
		isGltf_ = true;
	}
	else {
		isGltf_ = false;
	}
	
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

		//インデックスバッファの生成
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			IndexData indexData;
			indexData.index0 = face.mIndices[2];
			indexData.index1 = face.mIndices[1];
			indexData.index2 = face.mIndices[0];

			modelPart.modelData.indices.push_back(indexData);
		}

		//マテリアルのインプット
		unsigned int materialIndex = mesh->mMaterialIndex;
		const aiMaterial* material = scene->mMaterials[materialIndex];
		aiString textureFilePath;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
		modelPart.modelData.material.textureFilePath = filepath + "/" + textureFilePath.C_Str();
		std::string textureName = textureFilePath.C_Str();

		//もしテクスチャが見つからなかった場合は白い画像を入れる
		if (!textureName.empty()) {
			modelPart.textureHandle = TextureManager::Load(textureName, modelPart.modelData.material.textureFilePath);
		}
		else {
			modelPart.textureHandle = TextureManager::Load("whiteTexture2x2.png");
		}

		modelPart.name = mesh->mName.C_Str();

		meshs_.push_back(modelPart);
	}

	//アニメーションの読み込み
	//Noneの作成
	AnimationData animation;
	animation.name = "None";
	animation.numFrames = 0;
	animation.numChannels = 0;
	//GLTFからの読み込み
	for (uint32_t animationIndex = 0; animationIndex < scene->mNumAnimations; animationIndex++) {
		std::string animationName = scene->mAnimations[animationIndex]->mName.C_Str();
		AnimationData animation;
		animation.name = scene->mAnimations[animationIndex]->mName.C_Str();
		animation.numFrames = 0;
		animation.numChannels = scene->mAnimations[animationIndex]->mNumChannels;
		animation.channels.resize(scene->mAnimations[animationIndex]->mNumChannels);
		for (uint32_t channelIndex = 0; channelIndex < scene->mAnimations[animationIndex]->mNumChannels; channelIndex++) {
			//移動するNodeの名前
			std::string nodeName = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNodeName.C_Str();
			animation.channels[channelIndex].name = nodeName;
			//Frame
			if (scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumPositionKeys > animations_[animationName].numFrames) {
				animation.numFrames = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumPositionKeys;
			}
			if (scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumRotationKeys > animations_[animationName].numFrames) {
				animation.numFrames = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumRotationKeys;
			}
			if (scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumScalingKeys > animations_[animationName].numFrames) {
				animation.numFrames = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumScalingKeys;
			}

			//場所に関係する情報の格納場所
			animation.channels[channelIndex].numPositionChannel = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumPositionKeys;
			animation.channels[channelIndex].positionChannel.resize(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumPositionKeys);
			for (uint32_t keyIndex = 0; keyIndex < scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumPositionKeys; keyIndex++) {
				animation.channels[channelIndex].positionChannel[keyIndex].time = static_cast<float>(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mTime);
				animation.channels[channelIndex].positionChannel[keyIndex].position.x = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mValue.x;
				animation.channels[channelIndex].positionChannel[keyIndex].position.y = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mValue.y;
				animation.channels[channelIndex].positionChannel[keyIndex].position.z = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mValue.z;
			}
			//回転に関係する情報の格納場所
			animation.channels[channelIndex].numRotateChannel = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumRotationKeys;
			animation.channels[channelIndex].rotationChannel.resize(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumRotationKeys);
			for (uint32_t keyIndex = 0; keyIndex < scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumRotationKeys; keyIndex++) {
				animation.channels[channelIndex].rotationChannel[keyIndex].time = static_cast<float>(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mTime);
				animation.channels[channelIndex].rotationChannel[keyIndex].rotation.x = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mValue.x * 3.14f;
				animation.channels[channelIndex].rotationChannel[keyIndex].rotation.y = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mValue.y * 3.14f;
				animation.channels[channelIndex].rotationChannel[keyIndex].rotation.z = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mValue.z * 3.14f;
			}
			//サイズに関係する情報の格納場所
			animation.channels[channelIndex].numScaleChannel = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumScalingKeys;
			animation.channels[channelIndex].scaleChannel.resize(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumScalingKeys);
			for (uint32_t keyIndex = 0; keyIndex < scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumScalingKeys; keyIndex++) {
				animation.channels[channelIndex].scaleChannel[keyIndex].time = static_cast<float>(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mTime);
				animation.channels[channelIndex].scaleChannel[keyIndex].scale.x = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mValue.x;
				animation.channels[channelIndex].scaleChannel[keyIndex].scale.y = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mValue.y;
				animation.channels[channelIndex].scaleChannel[keyIndex].scale.z = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mValue.z;
			}

			animations_.push_back(animation);
		}

	}

	//Nodeの解析
	rootNode_ = ReadNode(scene->mRootNode);

	importer.FreeScene();
}