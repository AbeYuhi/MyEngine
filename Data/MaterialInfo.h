#pragma once
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Base/CreateResource/CreateResource.h"
#include "Transform.h"
#include "Material.h"
#include "MaterialData.h"

struct MaterialInfo {
	//Resource
	ComPtr<ID3D12Resource> resource_ = nullptr;
	//UVtransform
	TransformData uvTransform_;
	//マテリアル情報
	Material* material_;
	//MTLファイルの情報
	MaterialData materialData_;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// リソースの生成
	/// </summary>
	void CreateResource();

	/// <summary>
	/// MAPの実行
	/// </summary>
	void RunMap();

	/// <summary>
	/// アフィン変換
	/// </summary>
	void UpdateMatrix();
};