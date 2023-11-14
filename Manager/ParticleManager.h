#pragma once
#include "Data/RenderItem.h"

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
	ParticleManager();
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
	//DESCRIPTOR_HANDLE
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHadnelGPU_;
	//リソース
	ComPtr<ID3D12Resource> worldTransformResource_;
	//データ
	TransformMatrix* worldTransformData_;
	ParticleMaterialInfo materialInfo_;
	std::list<ParticleInfo> particles_;
	//ViewProjectionMatrix
	const Matrix4x4* viewProjectionMatrix_ = nullptr;
};