#pragma once
#include <memory>
#include <fstream>
#include <sstream>
#include <map>
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Vector4.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Data/VertexData.h"
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Data/MaterialData.h"
#include "DirectXGame/Data/ModelData.h"
#include "DirectXGame/Data/Material.h"
#include "DirectXGame/Data/MaterialInfo.h"
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/TextureData.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"

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

	void Draw(ParticleDrawInfo drawInfo);

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
	uint32_t textureHandle_;

	//オブジェクト情報のResource
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//オブジェクトのローカル情報
	VertexData* vertexData_;
};