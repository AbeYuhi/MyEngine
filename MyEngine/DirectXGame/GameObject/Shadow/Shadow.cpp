#include "Shadow.h"

void Shadow::Initialize() {
	//dxCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//Resourceの生成
	shadowResorce_ = CreateBufferResource(sizeof(Matrix4x4));

	//Resourceにデータを書き込む
	shadowResorce_->Map(0, nullptr, reinterpret_cast<void**>(&shadowData_));
}

void Shadow::Update(Vector3 lightVec) {

	DirectX::XMFLOAT4 planeVec(0, 1, 0, 1);
	DirectX::XMFLOAT4 light{};
	light.x = -lightVec.x;
	light.y = -lightVec.y;
	light.z = -lightVec.z;
	light.w = 0;
	DirectX::XMMATRIX data = DirectX::XMMatrixShadow(DirectX::XMLoadFloat4(&planeVec), DirectX::XMLoadFloat4(&light));
	
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			shadowData_->m[y][x] = DirectX::XMVectorGetByIndex(data.r[y], x);
		}
	}
}

void Shadow::Draw() {

}
