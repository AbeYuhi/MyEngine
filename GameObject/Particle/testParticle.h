#pragma once
#include "Manager/ParticleManager.h"
#include "Object/Plane.h"

class TestParticle : public ParticleManager
{
public:

	TestParticle();
	TestParticle(const Matrix4x4* viewProjectionMatrix, int maxParticleCount = 10);
	~TestParticle() = default;

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Plane> plane_;
};