#pragma once
#include <vector>
#include <map>
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Data/TextureData.h"
#include "External/DirectXTex/DirectXTex.h"
#include "External/DirectXTex/d3dx12.h"

class TextureManager
{
public:
	//インスタンスの取得
	static TextureManager* GetInstance();
	static const UINT kMaxTextureNum_ = 1000;
	static UINT sTextureNum_;

	static uint32_t Load(const std::string& textureName);

	static uint32_t Load(const std::string& textureName, const std::string& filePath);

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandleGPU(uint32_t textureHandle) { return textureDatas_[textureHandles_[textureHandle]].textureSrvHandleGPU; }
	
	D3D12_RESOURCE_DESC GetTextureDesc(uint32_t textureHandle) { return textureDatas_[textureHandles_[textureHandle]].textureResource->GetDesc(); }

private: //メンバ関数

	uint32_t LoadInternal(const std::string& textureName, const std::string& filePath);

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
	std::map<uint32_t, std::string> textureHandles_;
};