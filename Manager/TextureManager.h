#pragma once
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../External/DirectXTex/DirectXTex.h"

enum TextureName {
	 UVCHECKER,
	 TEXTURENUM
};

class TextureManager
{
public:
	//インスタンスの取得
	static TextureManager* GetInstance();

	void Initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandleGPU(UINT textureNum) { return textureSrvHandleGPU_[textureNum]; }

private: //メンバ関数

	void TransferTexture();

	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	void CreateShaderResourceView(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, int i);

public: //メンバ変数

	DirectX::ScratchImage mipImages_[TEXTURENUM];
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[TEXTURENUM];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[TEXTURENUM];
	ComPtr<ID3D12Resource> textureResources_[TextureName::TEXTURENUM];

private:
	TextureManager() = default;
	~TextureManager() = default;

};