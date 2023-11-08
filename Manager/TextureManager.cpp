#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

void TextureManager::Initialize() {
	TransferTexture();
}

void TextureManager::TransferTexture() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//画像の読み込み
	mipImages_[UVCHECKER] = LoadTexture("uvChecker.png");
	mipImages_[WHITE] = LoadTexture("whiteTexture2x2.png");
	mipImages_[MONSTERBALL] = LoadTexture("monsterBall.png");
	mipImages_[FENCE] = LoadTexture("fence.png");
	mipImages_[TEST] = LoadTexture("testPing.png");

	//Metadata
	DirectX::TexMetadata metadata[TEXTURENUM] = {};
	for (int i = 0; i < TEXTURENUM; i++) {
		metadata[i] = mipImages_[i].GetMetadata();
		textureResources_[i] = CreateTextureResource(metadata[i]);
		ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(textureResources_[i].Get(), mipImages_[i]);
		
		dxCommon->TransferCommandList();
	}

	for (int i = 0; i < TEXTURENUM; i++) {
		CreateShaderResourceView(textureResources_[i].Get(), mipImages_[i], textureSrvHandleCPU_[i], textureSrvHandleGPU_[i], i);
	}
}

DirectX::ScratchImage TextureManager::LoadTexture(const std::string&filePath) {
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString("Resources/Images/" + filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミニマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	return mipImages;
}

ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
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

ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	//DxCommonの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	std::vector<D3D12_SUBRESOURCE_DATA> subresource;
	DirectX::PrepareUpload(dxCommon->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresource);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresource.size()));
	ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(intermediateSize);
	UpdateSubresources(dxCommon->GetCommandList(), texture, intermediateResource.Get(), 0, 0, UINT(subresource.size()), subresource.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_GENERIC_READへResourceStateを変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dxCommon->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

void TextureManager::CreateShaderResourceView(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, D3D12_CPU_DESCRIPTOR_HANDLE& textureSrvHandleCPU, D3D12_GPU_DESCRIPTOR_HANDLE& textureSrvHandleGPU, int i) {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorの場所を決める
	textureSrvHandleCPU = dxCommon->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU = dxCommon->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	//先頭はImGuiが使っているのでその次を使う
	for (int j = 0; j < i + 1; j++) {
		textureSrvHandleCPU.ptr += dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		textureSrvHandleGPU.ptr += dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	//SRVの生成
	dxCommon->GetDevice()->CreateShaderResourceView(texture, &srvDesc, textureSrvHandleCPU);
}