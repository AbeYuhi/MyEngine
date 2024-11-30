#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

/// <summary>
/// スタティック変数の実体化
/// </summary>
UINT TextureManager::sTextureNum_ = 0;

TextureManager::~TextureManager() {
	for (auto& textureData : textureDatas_) {
		textureData.second.textureResource.Reset();
	}
}

uint32_t TextureManager::Load(const std::string& textureName) {
	return GetInstance()->LoadInternal("", textureName);
}

uint32_t TextureManager::Load(const std::string& filePath, const std::string& textureName) {
	return GetInstance()->LoadInternal(filePath, textureName);
}

uint32_t TextureManager::LoadInternal(const std::string& filePath, const std::string& textureName) {

	std::string fullFilePath = filePath + "/" + textureName;

	if (textureDatas_.find(fullFilePath) == textureDatas_.end()) {
		sTextureNum_++;
		if (sTextureNum_ > kMaxTextureNum_) {
			Log(ConvertString(std::format(L"テクスチャの最大読み込み枚数は1000枚です\n")));
			assert(false);
		}

		//テクスチャを読み込む
		TransferTexture(fullFilePath);

		//テクスチャの総数から一つ引く
		textureHandles_[sTextureNum_ - 1] = fullFilePath;
		return sTextureNum_ - 1;
	}
	else {
		Log(ConvertString(std::format(L"読み込み済みのテクスチャです\n")));
		return textureDatas_[fullFilePath].textureHandleIndex;
	}
}

void TextureManager::TransferTexture(const std::string& filePath) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//画像の読み込み
	textureDatas_[filePath].mipImage = LoadTexture(filePath);

	//Metadata
	DirectX::TexMetadata metadata = {};
	metadata = textureDatas_[filePath].mipImage.GetMetadata();
	textureDatas_[filePath].textureResource = CreateTextureResource(metadata);
	ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(filePath);

	dxCommon->TransferCommandList();

	CreateShaderResourceView(filePath);

	//テクスチャの総数から一つ引く
	textureDatas_[filePath].textureHandleIndex = sTextureNum_ - 1;
}

DirectX::ScratchImage TextureManager::LoadTexture(const std::string&filePath) {
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString("Resources/Images/" + filePath);
	HRESULT hr; 
	if (filePathW.ends_with(L".dds")) {
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else {
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}
	assert(SUCCEEDED(hr));

	//ミニマップの生成
	DirectX::ScratchImage mipImages{};
	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		mipImages = std::move(image);
	}
	else {
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	}
	assert(SUCCEEDED(hr));

	return mipImages;
}

ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(DirectX::TexMetadata metadata) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//Resourceの生成
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

ComPtr<ID3D12Resource> TextureManager::UploadTextureData(const std::string& filePath) {
	//DxCommonの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	std::vector<D3D12_SUBRESOURCE_DATA> subresource;
	DirectX::PrepareUpload(dxCommon->GetDevice(), textureDatas_[filePath].mipImage.GetImages(), textureDatas_[filePath].mipImage.GetImageCount(), textureDatas_[filePath].mipImage.GetMetadata(), subresource);
	uint64_t intermediateSize = GetRequiredIntermediateSize(textureDatas_[filePath].textureResource.Get(), 0, UINT(subresource.size()));
	ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(intermediateSize);
	UpdateSubresources(dxCommon->GetCommandList(), textureDatas_[filePath].textureResource.Get(), intermediateResource.Get(), 0, 0, UINT(subresource.size()), subresource.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_GENERIC_READへResourceStateを変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = textureDatas_[filePath].textureResource.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dxCommon->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

void TextureManager::CreateShaderResourceView(const std::string& filePath) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//Meta情報を取得
	const DirectX::TexMetadata& metadata = textureDatas_[filePath].mipImage.GetMetadata();
	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (metadata.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	}

	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const DirectX::Image* img = textureDatas_[filePath].mipImage.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		LRESULT hr = textureDatas_[filePath].textureResource->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch);
		assert(SUCCEEDED(hr));
	}

	uint32_t srvIndex = SrvManager::GetInstance()->Allocate();

	//SRVを作成するDescriptorの場所を決める
	textureDatas_[filePath].textureSrvHandleCPU = SrvManager::GetInstance()->GetCPUDescriptorHandle(srvIndex);
	textureDatas_[filePath].textureSrvHandleGPU = SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex);
	//SRVの生成
	dxCommon->GetDevice()->CreateShaderResourceView(textureDatas_[filePath].textureResource.Get(), &srvDesc, textureDatas_[filePath].textureSrvHandleCPU);
}