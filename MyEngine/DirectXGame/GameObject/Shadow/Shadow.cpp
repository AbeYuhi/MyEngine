#include "Shadow.h"

void Shadow::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//Resourceの生成
	shadowResorce_ = CreateBufferResource(sizeof(DirectX::XMMATRIX));

	//Resourceにデータを書き込む
	shadowResorce_->Map(0, nullptr, reinterpret_cast<void**>(&shadowData_));
}

void Shadow::Update(Vector3 lightVec) {

	DirectX::XMFLOAT4 planeVec(0, 1, 0, 0);
	DirectX::XMFLOAT3 light{};
	light.x = -lightVec.x;
	light.y = -lightVec.y;
	light.z = -lightVec.z;
	DirectX::XMMATRIX shadowData = DirectX::XMMatrixShadow(DirectX::XMLoadFloat4(&planeVec), DirectX::XMLoadFloat3(&light));
	*shadowData_ = shadowData;
}

void Shadow::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GraphicsPipelineManager* psoManager = GraphicsPipelineManager::GetInstance();
	//パイプラインステートの設定
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPSO());
	//ルートシグネチャの設定
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetRootSignature());
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(5, shadowResorce_->GetGPUVirtualAddress());
}
