#include "Transform.h"

/// <summary>
/// ワールドポジションの更新
/// </summary>
void WorldTransform::UpdateWorld() { matrix_->World_ = MakeAffineMatrix(data_.scale_, data_.rotate_, data_.translate_); }

/// <summary>
/// ワールドビュープロジェクションの更新
/// </summary>
/// <param name="viewProjectionMatrix"></param>
void WorldTransform::UpdateWVP(Matrix4x4 viewProjectionMatrix) { matrix_->WVP_ = Multiply(matrix_->World_, viewProjectionMatrix); }
