#include "SpriteParticle.h"

SpriteParticle::SpriteParticle(int maxParticleCount) : ParticleManager(maxParticleCount) {}

void SpriteParticle::Initialize() {
	//基本機能の初期化
	ParticleManager::Initialize();

	particleSprite_ = Sprite::Create();

	//使用するテクスチャの読み込み
	//textureHandle_ = TextureManager::Load("uvChecker.png");
	textureHandle_ = TextureManager::Load("circle.png");
	spriteData_.Initialize(textureHandle_);

	//エミッター情報
	isSpriteParticle_ = true;
	emitter_.transform.scale_ = {100, 100, 0};
	emitter_.transform.translate_ = { 640, 360, 0 };
	emitter_.count = 3;
	emitter_.frequency = 0.5;

	//ブレンドモード
	blendMode_ = kBlendModeAdd;
}

void SpriteParticle::Update() {

#ifdef _DEBUG
	ImGui::Begin("SpriteParticle");
	ImGui::SliderFloat3("EmitterPos", &emitter_.transform.translate_.x, 0, 1280);
	ImGui::SliderFloat3("EmitterScale", &emitter_.transform.scale_.x, 0, 500);
	ImGui::SliderInt("popCount", &emitter_.count, 0, 10);
	ImGui::Checkbox("isPop", &isPopParticle_);
	ImGui::Checkbox("isInvisible", &materialInfo_.isInvisible_);
	ImGui::Checkbox("isInvisibleEmitter", &emitterSpriteInfo_.materialInfo_.isInvisible_);
	int blendMode = blendMode_;
	const char* modes[] = { "None", "Normal", "Add", "SubTract", "MultiPly", "Screen" };
	ImGui::Combo("blendMode", &blendMode, modes, IM_ARRAYSIZE(modes));
	blendMode_ = static_cast<BlendMode>(blendMode);
	GraphicsPipelineManager::GetInstance()->SetBlendMode(blendMode_);
	ImGui::End();
#endif // _DEBUG

	//パーティクルの更新
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end();) {
		ParticleInfo* particle = &(*itParticle);
		//生存時間を過ぎたら
		if (particle->lifeTime <= particle->currenttime) {
			itParticle = particles_.erase(itParticle);
			continue;
		}

		//移動
		particle->srtData.translate_ += particle->velocity;
		particle->currenttime += kDeltaTime_;
		particle->color.w = 1.0f - (particle->currenttime / particle->lifeTime);

		itParticle++;
	}

	//パーティクルの転送
	ParticleManager::Update();
}

void SpriteParticle::EmitterDraw() {
	ParticleManager::EmitterDraw();
}

void SpriteParticle::Draw() {

	GraphicsPipelineManager::GetInstance()->SetBlendMode(blendMode_);

	particleSprite_->Draw(drawInfo_, spriteData_);

	GraphicsPipelineManager::GetInstance()->SetBlendMode(preBlendMode_);
}

ParticleInfo SpriteParticle::MakeNewParticle() {
	ParticleInfo particle{};
	particle.srtData.Initialize();
	particle.srtData.translate_ = {
		randomManager_->GetRandomNumber(-emitter_.transform.scale_.x / 2.0f, emitter_.transform.scale_.x / 2.0f),
		randomManager_->GetRandomNumber(-emitter_.transform.scale_.y / 2.0f, emitter_.transform.scale_.y / 2.0f),
		randomManager_->GetRandomNumber(-emitter_.transform.scale_.z / 2.0f, emitter_.transform.scale_.z / 2.0f) };
	particle.srtData.translate_ += emitter_.transform.translate_;
	particle.velocity = { randomManager_->GetRandomNumber(-10.0f, 10.0f), randomManager_->GetRandomNumber(-10.0f, 10.0f), 0 };
	particle.color = { randomManager_->GetRandomNumber(0.0f, 1.0f), randomManager_->GetRandomNumber(0.0f, 1.0f), randomManager_->GetRandomNumber(0.0f, 1.0f), 1.0f };
	particle.lifeTime = randomManager_->GetRandomNumber(1.0f, 3.0f);
	particle.currenttime = 0.0f;
	return particle;
}