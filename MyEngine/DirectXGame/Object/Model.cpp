#include "Model.h"
#include "DirectXGame/Data/RenderItem.h"

Model::Model() {}
Model::~Model() {}

std::map<std::string, std::shared_ptr<Model>> Model::sModels_;

std::shared_ptr<Model> Model::Create(const std::string& filepath, const std::string filename) {
	std::string filePath = filepath + "/" + filename;
	if (sModels_.find(filePath) == sModels_.end()) {
		sModels_[filePath] = std::make_shared<Model>();
		sModels_[filePath]->Initialize(filepath, filename);
	}
	
	return sModels_[filePath];
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
		mesh.indexResource = CreateBufferResource(sizeof(uint32_t) * mesh.modelData.indices.size());
		//頂点バッファビューを作成
		mesh.indexBufferView.BufferLocation = mesh.indexResource->GetGPUVirtualAddress();
		mesh.indexBufferView.SizeInBytes = static_cast<uint32_t>(sizeof(uint32_t) * mesh.modelData.indices.size());
		mesh.indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		//頂点リソースにデータを書き込む
		mesh.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&mesh.indexData));
		std::memcpy(mesh.indexData, mesh.modelData.indices.data(), sizeof(uint32_t) * mesh.modelData.indices.size());
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

	renderItem.Update();

	//ViewPortの設定
	dxCommon->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	dxCommon->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
	//プリミティブ形状を設定
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int meshIndex = 0;
	for (auto& mesh : meshs_) {
		if (isGltf_) {
			renderItem.UpdateGltf(mesh, meshIndex);
		}

		if (haveSkininng_) {
			//パイプラインステートの設定
			dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO(kSkinning));
			//ルートシグネチャの設定
			dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature(kSkinning));

			D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
				mesh.vertexBufferView,
				renderItem.animation_.skinClusters[mesh.name].influenceBufferView
			};

			//VBVの設定
			dxCommon->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);

			dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(5, renderItem.animation_.skinClusters[mesh.name].paletteSrvHandle.second);
		}
		else {
			//パイプラインステートの設定
			dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO());
			//ルートシグネチャの設定
			dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature());
			//VBVの設定
			dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		}

		//Lightの設定
		LightObjectManager::GetInstance()->Draw();
		MainCamera::GetInstance()->Draw();
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
		//dxCommon->GetCommandList()->DrawInstanced(UINT(mesh.modelData.vertices.size()), 1, 0, 0);
		dxCommon->GetCommandList()->DrawIndexedInstanced(UINT(mesh.modelData.indices.size()), 1, 0, 0, 0);
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

	renderItem.Update();

	//ViewPortの設定
	dxCommon->GetCommandList()->RSSetViewports(1, psoManager->GetViewPort());
	//Scirssorの設定
	dxCommon->GetCommandList()->RSSetScissorRects(1, psoManager->GetScissorRect());
	//プリミティブ形状を設定
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	int meshIndex = 0;
	for (auto& mesh : meshs_) {
		if (isGltf_) {
			renderItem.UpdateGltf(mesh, meshIndex);
		}

		if (haveSkininng_) {
			//パイプラインステートの設定
			dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO(kSkinning));
			//ルートシグネチャの設定
			dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature(kSkinning));

			D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
				mesh.vertexBufferView,
				renderItem.animation_.skinClusters[mesh.name].influenceBufferView
			};

			//VBVの設定
			dxCommon->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
			dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(5, renderItem.animation_.skinClusters[mesh.name].paletteSrvHandle.second);
		}
		else {
			//パイプラインステートの設定
			dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO());
			//ルートシグネチャの設定
			dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature());
			//VBVの設定
			dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		}

		//Lightの設定
		LightObjectManager::GetInstance()->Draw();
		MainCamera::GetInstance()->Draw();
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
		dxCommon->GetCommandList()->DrawIndexedInstanced(UINT(mesh.modelData.indices.size()), 1, 0, 0, 0);
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
		dxCommon->GetCommandList()->DrawIndexedInstanced(UINT(mesh.modelData.indices.size()), (UINT)*drawInfo.kMaxParticleCount_, 0, 0, 0);
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
		dxCommon->GetCommandList()->DrawIndexedInstanced(UINT(mesh.modelData.indices.size()), (UINT)*drawInfo.kMaxParticleCount_, 0, 0, 0);
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

		//バーテックスバッファの生成
		modelPart.modelData.vertices.resize(mesh->mNumVertices);
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			modelPart.modelData.vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
			modelPart.modelData.vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
			modelPart.modelData.vertices[vertexIndex].texcoord = { texcoord.x, texcoord.y };
		}

		//インデックスバッファの生成
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			for (unsigned int element = 0; element < face.mNumIndices; element++) {
				uint32_t vertexIndex = face.mIndices[element];
				modelPart.modelData.indices.push_back(vertexIndex);
			}
		}

		//ジョイントのデータ
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelPart.modelData.skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix({ scale.x, scale.y, scale.z }, Normalize({ rotate.x, -rotate.y, -rotate.z, rotate.w }), { -translate.x, translate.y, translate.z });
			jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
				jointWeightData.vertexWeights.push_back({bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId});
			}
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
			modelPart.textureHandle = TextureManager::Load(modelPart.modelData.material.textureFilePath, textureName);
		}
		else {
			modelPart.textureHandle = TextureManager::Load("whiteTexture2x2.png");
		}

		modelPart.name = mesh->mName.C_Str();

		meshs_.push_back(modelPart);
	}

	//アニメーションの読み込み
	//GLTFからの読み込み
	for (uint32_t animationIndex = 0; animationIndex < scene->mNumAnimations; animationIndex++) {
		std::string animationName = scene->mAnimations[animationIndex]->mName.C_Str();
		AnimationData animation;
		animation.name = scene->mAnimations[animationIndex]->mName.C_Str();
		animation.duration = float(scene->mAnimations[animationIndex]->mDuration / scene->mAnimations[animationIndex]->mTicksPerSecond);//時間の単位を秒に変換
		for (uint32_t channelIndex = 0; channelIndex < scene->mAnimations[animationIndex]->mNumChannels; channelIndex++) {
			//移動するNodeの名前
			std::string nodeName = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNodeName.C_Str();
			NodeAnimation animationInfo;
			animationInfo.nodeName = nodeName;
			animationInfo.isMeshNode = false;
			for (auto& mesh : meshs_) {
				if (animationInfo.nodeName == mesh.name) {
					animationInfo.isMeshNode = true;
				}
			}

			//場所に関係する情報の格納場所
			for (uint32_t keyIndex = 0; keyIndex < scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumPositionKeys; keyIndex++) {
				Keyframe<Vector3> pos;
				pos.time = static_cast<float>(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mTime / scene->mAnimations[animationIndex]->mTicksPerSecond);
				pos.value.x = -scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mValue.x;
				pos.value.y = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mValue.y;
				pos.value.z = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mPositionKeys[keyIndex].mValue.z;
				animationInfo.position.keyframes.push_back(pos);
			}
			//回転に関係する情報の格納場所
			for (uint32_t keyIndex = 0; keyIndex < scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumRotationKeys; keyIndex++) {
				Keyframe<Quaternion> rotate;
				rotate.time = static_cast<float>(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mTime / scene->mAnimations[animationIndex]->mTicksPerSecond);
				rotate.value.x = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mValue.x;
				rotate.value.y = -scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mValue.y;
				rotate.value.z = -scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mValue.z;
				rotate.value.w = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mRotationKeys[keyIndex].mValue.w;
				animationInfo.rotation.keyframes.push_back(rotate);
			}
			//サイズに関係する情報の格納場所
			for (uint32_t keyIndex = 0; keyIndex < scene->mAnimations[animationIndex]->mChannels[channelIndex]->mNumScalingKeys; keyIndex++) {
				Keyframe<Vector3> scale;
				scale.time = static_cast<float>(scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mTime / scene->mAnimations[animationIndex]->mTicksPerSecond);
				scale.value.x = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mValue.x;
				scale.value.y = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mValue.y;
				scale.value.z = scene->mAnimations[animationIndex]->mChannels[channelIndex]->mScalingKeys[keyIndex].mValue.z;
				animationInfo.scale.keyframes.push_back(scale);
			}

			animation.nodeAnimations.push_back(animationInfo);
		}

		animations_.push_back(animation);
	}

	haveSkininng_ = false;
	for (auto& animation : animations_) {
		for (auto& nodeAnimation : animation.nodeAnimations) {
			if (!nodeAnimation.isMeshNode) {
				haveSkininng_ = true;
				break;
			}
		}
		if (haveSkininng_) {
			break;
		}
	}

	//Nodeの解析
	rootNode_ = CreateRootNode(scene->mRootNode);

	importer.FreeScene();
}