#include "ParticleManager.h"

ParticleManager::ParticleManager(const Matrix4x4* viewProjectionMatrix, int maxParticleCount) : viewProjectionMatrix_(viewProjectionMatrix), kMaxParticleCount_(maxParticleCount){}

ParticleManager::~ParticleManager(){
	UnloadParticle();
}

int ParticleManager::sEmittersCount_ = 0;
std::map<int, bool> ParticleManager::isDrawing_;
void ParticleManager::Initialize() {
	//インスタンスの取得
	randomManager_ = RandomManager::GetInstance();
	//現在生成されているパーティクル数をインクリメント
	sEmittersCount_++;
	if (sEmittersCount_ > 500) {
		Log(ConvertString(std::format(L"500個以上パーティクルは生成できません\n")));
		assert(false);
	}

	//リソースの作成
	worldTransformResource_ = CreateBufferResource(sizeof(ParticleForGPU) * kMaxParticleCount_);

	//書き込むためのアドレスの取得
	worldTransformResource_->Map(0, nullptr, reinterpret_cast<void**>(&particleData_));
	//単位行列の書き込み
	for (int index = 0; index < 10; index++) {
		particleData_[index].WVP_ = MakeIdentity4x4();
		particleData_[index].World_ = MakeIdentity4x4();
		particleData_[index].color_ = { 1.0f, 1.0f, 1.0f, 0.0f };
	}

	materialInfo_.Initialize();

	//SRVの生成
	CreateSRV();

	//エミッターの初期化
	emitterInfo_.pos = { 0, 0, 0 };
	emitterInfo_.size = { 0, 0, 0 };

	//描画に必要なもの
	drawInfo_.Initialize(&srvHandle_, &materialInfo_, &particleCount_);
}

void ParticleManager::Update() {
	//転送データの書き込み
	int index = 0;
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end(); itParticle++) {
		ParticleInfo* particle = &(*itParticle);

		Matrix4x4 worldMatrix = MakeAffineMatrix(particle->srtData.scale_, particle->srtData.rotate_, particle->srtData.translate_);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, *viewProjectionMatrix_);

		particleData_[index].WVP_ = worldViewProjectionMatrix;
		particleData_[index].World_ = worldMatrix;
		particleData_[index].color_ = particle->color;
		index++;

		if (index >= kMaxParticleCount_) {
			particleCount_ = kMaxParticleCount_;
			return;
		}
	}

	//現在のパーティクルの粒子数
	particleCount_ = (int)particles_.size();
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
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	for (int i = 0; i < kEmittersMaxCount_; i++) {
		if (!isDrawing_[i]) {
			srvHandle_.CPUHandle = dxCommon->GetCPUDescriptorHandle(1001 + i);
			srvHandle_.GPUHandle = dxCommon->GetGPUDescriptorHandle(1001 + i);
			isDrawing_[i] = true;
			index_ = i;
			break;
		}
		if (i == kEmittersMaxCount_ - 1) {
			assert(false);
		}
	}
	dxCommon->GetDevice()->CreateShaderResourceView(worldTransformResource_.Get(), &srvDesc, srvHandle_.CPUHandle);
}