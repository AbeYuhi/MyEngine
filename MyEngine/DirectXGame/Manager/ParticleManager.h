#pragma once
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/ResourceHandles.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"

struct ParticleInfo {
	TransformData srtData_;
	Vector3 velocity_;
};

class ParticleManager
{
private:
	//現在のパーティクルの生成数
	static int particleCount_;

public: //メンバ関数
	ParticleManager(const Matrix4x4* viewProjectionMatrix, int maxParticleCount);
	~ParticleManager();

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

private: //メンバ関数

	void CreateSRV();

protected: //メンバ変数
	//1つのパーティクルの最大粒子数
	const int kMaxParticleCount_;
	//リソース
	ComPtr<ID3D12Resource> worldTransformResource_;
	//Resourceハンドル
	ResourceHandles srvHandle_;
	//データ
	TransformMatrix* worldTransformData_;
	ParticleMaterialInfo materialInfo_;
	std::list<ParticleInfo> particles_;
	//描画の際に必要なパーティクルデータ
	ParticleDrawInfo drawInfo_;
	//ViewProjectionMatrix
	const Matrix4x4* viewProjectionMatrix_ = nullptr;
};