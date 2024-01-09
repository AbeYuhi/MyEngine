#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Manager/ObjectManager.h"
#include "DirectXGame/Object/Plane.h"
#include "DirectXGame/Object/Model.h"

class PlaneParticle : public ParticleManager
{
public:

	PlaneParticle(int maxParticleCount);
	~PlaneParticle() = default;

	void Initialize() override;

	void Update() override;

	void EmitterDraw() override;

	void Draw() override;

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	Model* particleModel_;
	uint32_t textureHandle_;

	//風フィールド
	AccelerationField accelerationField_;

	//風が吹いているか
	bool isAccelerationField_;
};

