#pragma once
#include <memory>
#include <fstream>
#include <sstream>
#include <map>
#include "Base/CreateResource/CreateResource.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/ImGuiManager.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector3_Math.hpp"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Data/VertexData.h"
#include "Data/Transform.h"
#include "Data/MaterialData.h"
#include "Data/ModelData.h"
#include "Data/Material.h"
#include "Data/MaterialInfo.h"
#include "Data/RenderItem.h"
#include "Data/TextureData.h"
#include "Manager/TextureManager.h"
#include "Manager/GraphicsPipelineManager.h"

class Model
{
public: //静的メンバ関数

	static std::unique_ptr<Model> Create(const std::string filename);

private: //静的メンバ変数

public: //メンバ関数
	Model();
	~Model();

	void Initialize(const std::string filename);

	void Draw(RenderItem& renderItem);

public: //ゲッターセッター

	//void SetColor(Vector4 color) { materialData_->color = color; }

private: //メンバ関数
	void LoadObjFile(const std::string& filename);
	void LoadMaterialDataTemplateFile(const std::string& folderPath, const std::string& filename);

private: //メンバ変数
	//モデルデータ
	ModelData modelData_;

	//テクスチャデータ
	std::string textureName_;

	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	VertexData* vertexData_;
};