#pragma once
#include <vector>
#include <map>
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/CreateResource/CreateResource.h"
#include "Data/TextureData.h"
#include "External/DirectXTex/DirectXTex.h"
#include "External/DirectXTex/d3dx12.h"

class TextureManager
{
public:
	//インスタンスの取得
	static TextureManager* GetInstance();
	static const UINT kMaxTextureNum_ = 100;
	static UINT sTextureNum_;

	void Initialize();

	void LoadTextures();

	void Load(const std::string& textureName, const std::string& filePath);

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandleGPU(std::string textureName) { return textureDatas_[textureName].textureSrvHandleGPU; }

private: //メンバ関数

	void TransferTexture(const std::string& textureName, const std::string& filePath);

	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	ComPtr<ID3D12Resource> CreateTextureResource(DirectX::TexMetadata metadata);

	[[nodiscard]]
	ComPtr<ID3D12Resource> UploadTextureData(const std::string& textureName);

	void CreateShaderResourceView(const std::string& textureName);

private:
	TextureManager() = default;
	~TextureManager() = default;

	std::map<std::string, TextureData> textureDatas_;
};