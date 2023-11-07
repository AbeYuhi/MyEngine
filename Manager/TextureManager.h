#pragma once
#include <vector>
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/CreateResource/CreateResource.h"
#include "External/DirectXTex/DirectXTex.h"
#include "External/DirectXTex/d3dx12.h"

enum TextureName {
	 UVCHECKER,
	 WHITE,
	 MONSTERBALL,
	 TEXTURENUM
};

class TextureManager
{
public:
	//インスタンスの取得
	static TextureManager* GetInstance();

	void Initialize();

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandleGPU(UINT textureNum) { return textureSrvHandleGPU_[textureNum]; }

	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	[[nodiscard]]
	ComPtr<ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	void CreateShaderResourceView(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, D3D12_CPU_DESCRIPTOR_HANDLE& textureSrvHandleCPU, D3D12_GPU_DESCRIPTOR_HANDLE& textureSrvHandleGPU, int i);

private: //メンバ関数

	void TransferTexture();

public: //メンバ変数

	DirectX::ScratchImage mipImages_[TEXTURENUM];
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[TEXTURENUM];
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[TEXTURENUM];
	ComPtr<ID3D12Resource> textureResources_[TextureName::TEXTURENUM];

private:
	TextureManager() = default;
	~TextureManager() = default;

};