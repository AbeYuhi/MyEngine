#include "MainCamera.h"

MainCamera* MainCamera::GetInstance() {
	static MainCamera instance;
	return &instance;
}

void MainCamera::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	viewProjectionMatrix_ = MakeIdentity4x4();

	//Resourceの生成
	cameraResorce_ = CreateBufferResource(sizeof(CameraForGPU));

	//Resourceにデータを書き込む
	cameraResorce_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = {0};
}

void MainCamera::Update(Matrix4x4 worldMatrix, Matrix4x4 projectionMatrix) {
	worldMatrix_ = worldMatrix;
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = projectionMatrix;
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
	cameraData_->worldPosition = { worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] };
}

void MainCamera::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO());
	//ルートシグネチャの設定
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature());
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, cameraResorce_->GetGPUVirtualAddress());
}