#pragma once
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Base/CreateResource/CreateResource.h"

struct TransformMatrix {
	Matrix4x4 WVP_;
	Matrix4x4 World_;
};

struct TransformData{
	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;

	void Initialize();
};

struct WorldTransform {
	//リソース
	ComPtr<ID3D12Resource> resource_ = nullptr;
	//ローカル情報
	TransformData data_;
	//ワールドMatrix
	Matrix4x4 worldMatrix_;
	//Matrix転送済みデータ
	TransformMatrix* matrix_;
	//親のワールドポインタ
	const WorldTransform* parent_ = nullptr;
	//ビュープロジェクションのポインタ
	const Matrix4x4* viewProjectionMatrix_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Matrix4x4* viewProjectionMatrix);

	/// <summary>
	/// 更新
	/// </summary>
	void UpdateWorld();

	/// <summary>
	/// 転送
	/// </summary>
	void TransferMatrix();

private:
	/// <summary>
	/// リソースの生成
	/// </summary>
	void CreateResource();
	
	/// <summary>
	/// MAPの実行
	/// </summary>
	void RunMap();
};