#pragma once
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"

struct TransformMatrix {
	Matrix4x4 WVP_;
	Matrix4x4 World_;
	Matrix4x4 WorldInverseTranspose_;
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
	void Initialize(bool isSprite);

	/// <summary>
	/// 更新
	/// </summary>
	void UpdateWorld();

	/// <summary>
	/// 転送
	/// </summary>
	void TransferMatrix();

	/// <summary>
	/// Nodeを含めた更新
	/// </summary>
	void NodeUpdate(Matrix4x4 localMatrix);

	/// <summary>
	/// ワールドポジションの取得
	/// </summary>
	Vector3 GetWorldPos();

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