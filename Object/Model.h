#pragma once
#include <memory>
#include <fstream>
#include <sstream>
#include "../Base/CreateResource/CreateResource.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Manager/ImGuiManager.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector3_Math.hpp"
#include "../Math/Vector4.h"
#include "../Math/Matrix4x4.h"
#include "../Data/VertexData.h"
#include "../Data/Transform.h"
#include "../Data/MaterialData.h"
#include "../Data/ModelData.h"
#include "../Data/Material.h"
#include "../Manager/TextureManager.h"
#include "../Manager/GraphicsPipelineManager.h"

class Model
{
public: //静的メンバ関数

	static std::unique_ptr<Model> Create(const std::string filename);

private: //静的メンバ変数
	static UINT sModelNum_;

public: //メンバ関数
	Model();
	~Model();

	void Initialize(const std::string filename);

	void Draw(WorldTransform& transform);

public: //ゲッターセッター

	//void SetColor(Vector4 color) { materialData_->color = color; }

private: //メンバ関数
	void LoadObjFile(const std::string& filename);
	void LoadMaterialDataTemplateFile(const std::string& folderPath, const std::string& filename);

private: //メンバ変数
	//モデルデータ
	ModelData modelData_;

	//テクスチャデータ
	DirectX::ScratchImage mipImages_;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	ComPtr<ID3D12Resource> textureResources_;

	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	TransformData uvTransform_;
	VertexData* vertexData_;
	Material* material_;
};