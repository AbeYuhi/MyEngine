#include "testParticle.h"


TestParticle::TestParticle(){}
TestParticle::TestParticle(const Matrix4x4* viewProjectionMatrix, int maxParticleCount) : ParticleManager(viewProjectionMatrix, maxParticleCount){}

void TestParticle::Initialize() {
	//基本機能の初期化
	ParticleManager::Initialize();

	//パーティクルの初期化
	plane_ = Plane::Create();
	plane_->Initialize();

	int index = 0;
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end(); itParticle++) {
		ParticleInfo* particle = &(*itParticle);
		index++;
		particle->srtData_.translate_ = { 0.1f * index, 0, 100 };
	}
}

void TestParticle::Update() {

	//パーティクルの更新
	ParticleManager::Update();
}

void TestParticle::Draw() {
	plane_->Draw(materialInfo_, kMaxParticleCount_, srvHadnelGPU_);
}