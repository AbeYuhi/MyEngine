#include "testParticle.h"


TestParticle::TestParticle(const Matrix4x4* viewProjectionMatrix, int maxParticleCount) : ParticleManager(viewProjectionMatrix, maxParticleCount){}

void TestParticle::Initialize() {
	//基本機能の初期化
	ParticleManager::Initialize();

	//パーティクルの初期化
	plane_ = Plane::Create();

	//使用するテクスチャの読み込み
	//textureHandle_ = TextureManager::Load("uvChecker.png");
	textureHandle_ = TextureManager::Load("circle.png");

	//エミッター情報
	emitterInfo_.size = {2, 2, 2};

	//パーティクルの生成
	for (int index = 0; index < 10; index++) {
		particles_.push_back(MakeNewParticle());
	}
}

void TestParticle::Update() {

	if (popCoolDown_ <= 0) {
		particles_.push_back(MakeNewParticle());
		popCoolDown_ = 10;
	}
	else {
		popCoolDown_--;
	}

	ImGui::Begin("EmitterPos");
	ImGui::SliderFloat3("pos", &emitterInfo_.pos.x, -10, 10);
	ImGui::End();

	//パーティクルの更新
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end();) {
		ParticleInfo* particle = &(*itParticle);
		//生存時間を過ぎたら
		if (particle->lifeTime <= particle->currenttime) {
			itParticle = particles_.erase(itParticle);
			continue;
		}

		const float kDeltaTime = 1.0f / 60.0f;
		particle->srtData.translate_ += particle->velocity * kDeltaTime;
		particle->currenttime += kDeltaTime;
		particle->color.w = 1.0f - (particle->currenttime / particle->lifeTime);

		itParticle++;
	}

	//パーティクルの転送
	ParticleManager::Update();
}

void TestParticle::Draw() {
	GraphicsPipelineManager::GetInstance()->SetBlendMode(kBlendModeAdd);
	plane_->Draw(drawInfo_, textureHandle_);
	GraphicsPipelineManager::GetInstance()->SetBlendMode(kBlendModeNormal);
}

ParticleInfo TestParticle::MakeNewParticle() {
	ParticleInfo particle{};
	particle.srtData.Initialize();
	particle.srtData.translate_ = { 
		randomManager_->GetRandomNumber(-emitterInfo_.size.x / 2.0f, emitterInfo_.size.x / 2.0f),
		randomManager_->GetRandomNumber(-emitterInfo_.size.y / 2.0f, emitterInfo_.size.y / 2.0f),
		randomManager_->GetRandomNumber(-emitterInfo_.size.z / 2.0f, emitterInfo_.size.z / 2.0f) };
	particle.srtData.translate_ += emitterInfo_.pos;
	particle.velocity = { randomManager_->GetRandomNumber(-1.0f, 1.0f), randomManager_->GetRandomNumber(-1.0f, 1.0f), randomManager_->GetRandomNumber(-1.0f, 1.0f) };
	particle.color = { randomManager_->GetRandomNumber(0.0f, 1.0f), randomManager_->GetRandomNumber(0.0f, 1.0f), randomManager_->GetRandomNumber(0.0f, 1.0f), 1.0f };
	particle.lifeTime = randomManager_->GetRandomNumber(1.0f, 3.0f);
	particle.currenttime = 0.0f;
	return particle;
}