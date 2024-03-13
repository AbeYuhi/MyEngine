#pragma once
#include <map>
#include <format>
#include <numbers>
#include "DirectXGame/Data/RenderItem.h"
#include "DirectXGame/Data/ResourceHandles.h"
#include "DirectXGame/Data/ParticleDrawInfo.h"
#include "DirectXGame/Data/ParticleForGPU.h"
#include "DirectXGame/Data/AccelerationField.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Manager/RandomManager.h"
#include "DirectXGame/Object/WireFrameBox.h"
#include "DirectXGame/Object/WireFrameSpriteBox.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"

struct ParticleInfo {
	TransformData srtData;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currenttime;
};

struct Emitter {
	TransformData transform;
	int count; //発生数
	float frequency;
	float frequencyTime;
};

class ParticleManager
{
private:
	//現在のパーティクルの生成数
	static int sEmittersCount_;
	static const int kEmittersMaxCount_ = 1000;
	static std::map<int, bool> isDrawing_;

public: //メンバ関数
	ParticleManager(int maxParticleCount);
	~ParticleManager();

	virtual void Initialize();

	virtual void Update();

	virtual void EmitterDraw();

	virtual void Draw();

	virtual ParticleInfo MakeNewParticle() = 0;

	void PopParticle();

	void UnloadParticle();

private: //メンバ関数

	void CreateSRV();
	std::list<ParticleInfo> Emission();

public: //ゲッターセッター

	inline bool GetIsPopParticle() { return isPopParticle_; }
	inline void SetIsPopParticle(bool isPopParticle) { isPopParticle_ = isPopParticle; }
	inline void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }
	inline void SetEmitterPos(Vector3 pos) { emitter_.transform.translate_ = pos; }

protected: //メンバ変数
	//乱数マネージャー
	RandomManager* randomManager_;
	//1つのパーティクルの最大粒子数
	const int kMaxParticleCount_;
	//現在のパーティクルの粒子数
	int particleCount_;
	//ブレンドモード
	BlendMode blendMode_;
	BlendMode preBlendMode_;
	//テクスチャハンドル
	uint32_t textureHandle_;

	//リソース
	ComPtr<ID3D12Resource> worldTransformResource_;
	int index_;
	//Resourceハンドル
	ResourceHandles srvHandle_;
	//エミッターの情報
	Emitter emitter_;
	std::shared_ptr<WireFrameBox> emitterObj_;
	std::shared_ptr<WireFrameSpriteBox> emitterSprite_;
	RenderItem emitterObjInfo_;
	SpriteItem emitterSpriteInfo_;
	//データ
	ParticleForGPU* particleData_;
	ParticleMaterialInfo materialInfo_;
	std::list<ParticleInfo> particles_;

	//スプライトのパーティクルかどうか
	bool isSpriteParticle_;
	//パーティクルを発生させるか
	bool isPopParticle_;
	//ビルボードするかどうか
	bool isBillboard_;
	
	//描画の際に必要なパーティクルデータ
	ParticleDrawInfo drawInfo_;
	//時間
	const float kDeltaTime_ = 1.0f / 60.0f;
};