#include "ParticleManager.h"

ParticleManager::ParticleManager(int maxParticleCount) : kMaxParticleCount_(maxParticleCount){}

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
	if (sEmittersCount_ > 1000) {
		Log(ConvertString(std::format(L"1000個以上パーティクルは生成できません\n")));
		assert(false);
	}

	//リソースの作成
	worldTransformResource_ = CreateBufferResource(sizeof(ParticleForGPU) * kMaxParticleCount_);

	//書き込むためのアドレスの取得
	worldTransformResource_->Map(0, nullptr, reinterpret_cast<void**>(&particleData_));
	//単位行列の書き込み
	/*for (int index = 0; index < 10; index++) {
		particleData_[index].WVP_ = MakeIdentity4x4();
		particleData_[index].World_ = MakeIdentity4x4();
		particleData_[index].color_ = { 1.0f, 1.0f, 1.0f, 0.0f };
	}*/

	materialInfo_.Initialize();

	//SRVの生成
	CreateSRV();

	//エミッターの初期化
	emitter_.transform.Initialize();
	emitter_.count = 1;
	emitter_.frequency = 0.5f;
	emitter_.frequencyTime = 0.0f;

	emitterObj_ = WireFrameBox::Create();
	emitterObjInfo_.Initialize();

	emitterSprite_ = WireFrameSpriteBox::Create();
	emitterSpriteInfo_.Initialize(1, {1, 1});

#ifdef NDEBUG
	emitterObjInfo_.materialInfo_.isInvisible_ = true;
	emitterSpriteInfo_.materialInfo_.isInvisible_ = true;
#endif // NDEBUG

	//パーティクルがスプライト用かどうか
	isSpriteParticle_ = false;

	//パーティクルを発生させるか
	isPopParticle_ = true;

	//ビルボードさせるか
	isBillboard_ = true;

	//ブレンドモード
	blendMode_ = kBlendModeAdd;

	//描画に必要なもの
	drawInfo_.Initialize(&srvHandle_, &materialInfo_, &particleCount_);
}

void ParticleManager::Update() {
	preBlendMode_ = GraphicsPipelineManager::GetInstance()->GetBlendMode();

	//エミッターの可視化
	emitterObjInfo_.worldTransform_.data_ = emitter_.transform;
	emitterObjInfo_.Update();

	emitterSpriteInfo_.worldTransform_.data_ = emitter_.transform;
	emitterSpriteInfo_.Update();

	//パーティクルの発生
	emitter_.frequencyTime += kDeltaTime_;
	if (emitter_.frequency <= emitter_.frequencyTime) {
		if (isPopParticle_) {
			particles_.splice(particles_.end(), Emission());
		}
		emitter_.frequencyTime = 0;
	}

	//ビルボードの計算
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, MainCamera::GetInstance()->GetWorldMatrix());
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	//転送データの書き込み
	int index = 0;
	for (std::list<ParticleInfo>::iterator itParticle = particles_.begin(); itParticle != particles_.end(); itParticle++) {
		ParticleInfo* particle = &(*itParticle);

		//ワールドマトリックスの計算
		Matrix4x4 worldMatrix = MakeIdentity4x4();
		Matrix4x4 worldViewProjectionMatrix = MakeIdentity4x4();
		if (isSpriteParticle_) {
			worldMatrix = MakeAffineMatrix(particle->srtData.scale_, particle->srtData.rotate_, particle->srtData.translate_);
			worldViewProjectionMatrix = Multiply(worldMatrix, SpriteCamera::GetInstance()->GetViewProjectionMatrix());
		}
		else {
			if (isBillboard_) {
				worldMatrix = Multiply(MakeScaleMatrix(particle->srtData.scale_), Multiply(billboardMatrix, MakeTranslateMatrix(particle->srtData.translate_)));
			}
			else {
				worldMatrix = MakeAffineMatrix(particle->srtData.scale_, particle->srtData.rotate_, particle->srtData.translate_);
			}
			worldViewProjectionMatrix = Multiply(worldMatrix, MainCamera::GetInstance()->GetViewProjectionMatrix());
		}

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

void ParticleManager::EmitterDraw() {

	if (isSpriteParticle_) {
		if (emitterSpriteInfo_.materialInfo_.isInvisible_) {
			return;
		}

		emitterSprite_->Draw(emitterSpriteInfo_);
	}
	else {
		if (emitterObjInfo_.materialInfo_.isInvisible_) {
			return;
		}

		emitterObj_->Draw(emitterObjInfo_);
	}
}

void ParticleManager::Draw() {}

void ParticleManager::PopParticle() {
	particles_.push_back(MakeNewParticle());
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

std::list<ParticleInfo> ParticleManager::Emission() {
	std::list<ParticleInfo> particles;
	for (int count = 0; count < emitter_.count; count++) {
		particles.push_back(MakeNewParticle());
	}
	return particles;
}