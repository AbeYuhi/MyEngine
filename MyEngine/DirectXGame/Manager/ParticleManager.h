#pragma once
#include <map>
#include <format>
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/ResourceHandles.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"
#include "DirectXGame/Data/ParticleForGPU.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Manager/RandomManager.h"

struct ParticleInfo {
	TransformData srtData;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currenttime;
};

struct EmitterInfo {
	Vector3 pos;
	Vector3 size;
};

class ParticleManager
{
private:
	//現在のパーティクルの生成数
	static int sEmittersCount_;
	static const int kEmittersMaxCount_ = 500;
	static std::map<int, bool> isDrawing_;

public: //メンバ関数
	ParticleManager(const Matrix4x4* viewProjectionMatrix, int maxParticleCount);
	~ParticleManager();

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	virtual ParticleInfo MakeNewParticle() = 0;

	void UnloadParticle();

private: //メンバ関数

	void CreateSRV();

protected: //メンバ変数
	//乱数マネージャー
	RandomManager* randomManager_;
	//1つのパーティクルの最大粒子数
	const int kMaxParticleCount_;
	//現在のパーティクルの粒子数
	int particleCount_;
	//リソース
	ComPtr<ID3D12Resource> worldTransformResource_;
	int index_;
	//Resourceハンドル
	ResourceHandles srvHandle_;
	//エミッターの情報
	EmitterInfo emitterInfo_;
	//データ
	ParticleForGPU* particleData_;
	ParticleMaterialInfo materialInfo_;
	std::list<ParticleInfo> particles_;
	//描画の際に必要なパーティクルデータ
	ParticleDrawInfo drawInfo_;
	//ViewProjectionMatrix
	const Matrix4x4* viewProjectionMatrix_ = nullptr;
};