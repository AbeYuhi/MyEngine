#include "ParticleManager.h"

ParticleManager::ParticleManager(const Matrix4x4* viewProjectionMatrix, int maxParticleCount) : viewProjectionMatrix_(viewProjectionMatrix), kMaxParticleCount_(maxParticleCount){}

ParticleManager::~ParticleManager(){
	UnloadParticle();
}

int ParticleManager::particleCount_ = 0;
std::map<int, bool> ParticleManager::isDrawing_;
void ParticleManager::Initialize() {
	particleCount_++;
	if (particleCount_ > 500) {
		Log(ConvertString(std::format(L"500個以上パーティクルは生成できません\n")));
		assert(false);
	}

	//リソースの作成
	worldTransformResource_ = CreateBufferResource(sizeof(ParticleTransformMatrix) * kMaxParticleCount_);

	//書き込むためのアドレスの取得
	worldTransformResource_->Map(0, nullptr, reinterpret_cast<void**>(&worldTransformData_));
	//単位行列の書き込み
	for (int index = 0; index < kMaxParticleCount_; index++) {
		worldTransformData_[index].WVP_ = MakeIdentity4x4();
		worldTransformData_[index].World_ = MakeIdentity4x4();
	}

	materialInfo_.Initialize();

	//SRVの生成
	CreateSRV();

	//初期化
	for (int index = 0; index < kMaxParticleCount_; index++) {
		ParticleInfo particle{};
		particle.srtData_.Initialize();
		particle.velocity_ = { 0, 0, 0 };
		particles_.push_back(particle);
	}

	//描画に必要なもの
	drawInfo_.Initialize(&srvHandle_, &materialInfo_, &kMaxParticleCount_);
}

void ParticleManager::Update() {
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end(); itParticle++) {
		ParticleInfo* particle = &(*itParticle);
		const float kDeltaTime = 1.0f / 60.0f;
		particle->srtData_.translate_ += particle->velocity_ * kDeltaTime;
	}

	int index = 0;
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end(); itParticle++) {
		ParticleInfo* particle = &(*itParticle);

		Matrix4x4 worldMatrix = MakeAffineMatrix(particle->srtData_.scale_, particle->srtData_.rotate_, particle->srtData_.translate_);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, *viewProjectionMatrix_);

		worldTransformData_[index].WVP_ = worldViewProjectionMatrix;
		worldTransformData_[index].World_ = worldMatrix;
		index++;

		if (index >= kMaxParticleCount_) {
			break;
		}
	}
}

void ParticleManager::Draw() {

}

void ParticleManager::UnloadParticle() {
	particleCount_--;
	isDrawing_[index_] = false;
	worldTransformResource_.Reset();
}

void ParticleManager::CreateSRV() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kMaxParticleCount_;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleTransformMatrix);
	for (int i = 0; i < particleMaxCount_; i++) {
		if (!isDrawing_[i]) {
			srvHandle_.CPUHandle = dxCommon->GetCPUDescriptorHandle(1001 + i);
			srvHandle_.GPUHandle = dxCommon->GetGPUDescriptorHandle(1001 + i);
			isDrawing_[i] = true;
			index_ = i;
			break;
		}
		if (i == particleMaxCount_ - 1) {
			assert(false);
		}
	}
	dxCommon->GetDevice()->CreateShaderResourceView(worldTransformResource_.Get(), &srvDesc, srvHandle_.CPUHandle);
}