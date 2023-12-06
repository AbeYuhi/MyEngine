#pragma once
#include "DirectXGame/Manager/ParticleManager.h"
#include "DirectXGame/Object/Plane.h"
#include "DirectXGame/Object/Model.h"

class TestParticle : public ParticleManager
{
public:

	TestParticle(const Matrix4x4* viewProjectionMatrix, int maxParticleCount);
	~TestParticle() = default;

	void Initialize() override;

	void Update() override;

	void Draw() override;

	ParticleInfo MakeNewParticle() override;

private:
	std::unique_ptr<Plane> plane_;
	uint32_t textureHandle_;

	int popCoolDown_;
};