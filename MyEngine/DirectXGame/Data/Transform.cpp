#include "Transform.h"
#include "DirectXGame/GameObject/Camera/MainCamera.h"
#include "DirectXGame/GameObject/Camera/SpriteCamera.h"

void TransformData::Initialize(){
	scale_ = { 1.0f, 1.0f, 1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, 0.0f };
}

void WorldTransform::Initialize(bool isSprite) {
	CreateResource();
	RunMap();

	data_.Initialize();

	if (isSprite) {
		viewProjectionMatrix_ = SpriteCamera::GetInstance()->GetPViewProjectionMatrix();
	}
	else {
		viewProjectionMatrix_ = MainCamera::GetInstance()->GetPViewProjectionMatrix();
	}

	UpdateWorld();
}

void WorldTransform::CreateResource() {
	//transformationMatrixResourceの生成
	resource_ = CreateBufferResource(sizeof(TransformMatrix));
}

void WorldTransform::RunMap() {
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&matrix_));
}

void WorldTransform::UpdateWorld() { 
	worldMatrix_ = MakeAffineMatrix(data_.scale_, data_.rotate_, data_.translate_);
	if (parent_) {
		worldMatrix_ = Multiply(worldMatrix_, parent_->worldMatrix_);
	}

	TransferMatrix();
}

void WorldTransform::TransferMatrix() {
	matrix_->World_ = worldMatrix_;
	matrix_->WVP_ = Multiply(matrix_->World_, *viewProjectionMatrix_);
	matrix_->WorldInverseTranspose_ = Transpose(Inverse(worldMatrix_));
}

void WorldTransform::NodeUpdate(Matrix4x4 localMatrix) {
	matrix_->World_ = Multiply(localMatrix, worldMatrix_);
	matrix_->WVP_ = Multiply(Multiply(localMatrix, worldMatrix_), *viewProjectionMatrix_);
	matrix_->WorldInverseTranspose_ = Transpose(Inverse(worldMatrix_));
}

Vector3 WorldTransform::GetWorldPos() {
	Vector3 pos;
	pos.x = worldMatrix_.m[3][0];
	pos.y = worldMatrix_.m[3][1];
	pos.z = worldMatrix_.m[3][2];
	return pos;
}