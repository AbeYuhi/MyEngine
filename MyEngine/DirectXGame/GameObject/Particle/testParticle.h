#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Manager/ObjectManager.h"
#include "DirectXGame/Object/Plane.h"
#include "DirectXGame/Object/Model.h"

class TestParticle : public ParticleManager
{
public:

	TestParticle(int maxParticleCount);
	~TestParticle() = default;

	void Initialize() override;

	void Update() override;

	void EmitterDraw() override;

	void Draw() override;

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	//std::unique_ptr<Plane> plane_;
	Model* particleModel_;
	uint32_t textureHandle_;

	//風フィールド
	AccelerationField accelerationField_;

	//風が吹いているか
	bool isAccelerationField_;
};