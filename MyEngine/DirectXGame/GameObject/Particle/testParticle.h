#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Object/Plane.h"
#include "DirectXGame/Object/Model.h"

class TestParticle : public ParticleManager
{
public:

	TestParticle(int maxParticleCount);
	~TestParticle() = default;

	void Initialize() override;

	void Update() override;

	void Draw() override;

private: //メンバ関数

	ParticleInfo MakeNewParticle() override;

private:
	std::unique_ptr<Plane> plane_;
	uint32_t textureHandle_;
};