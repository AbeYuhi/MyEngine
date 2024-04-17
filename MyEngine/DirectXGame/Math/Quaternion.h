#pragma once
#include <math.h>
#include "Vector3.h"
#include "Matrix4x4.h"

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

Quaternion operator*(const Quaternion& num1, const float num2);
Quaternion operator*(const float num1, const Quaternion& num2);
Quaternion operator+(const Quaternion& num1, const Quaternion& num2);
bool operator==(const Quaternion& num1, const Quaternion& num2);

//積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
//単位
Quaternion IdentityQuaternion();
//共役
Quaternion Conjugate(const Quaternion& quaternion);
//ノルム
float Norm(const Quaternion& quaternion);
//正規化
Quaternion Normalize(const Quaternion& quaternion);
//逆
Quaternion Inverse(const Quaternion& quaternion);
//内積
float Dot(const Quaternion& q0, const Quaternion& q1);

//任意回転軸を表すQuaternion
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

//ベクトルをQuaternionで回転させた結果のベクトル
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

//回転行列
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

Quaternion Slerp(Quaternion q0, Quaternion q1, float t);