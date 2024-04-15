#include "Quaternion.h"

Quaternion operator*(const Quaternion& num1, const float num2) {
	Quaternion ans;
	ans.w = num1.w * num2;
	ans.x = num1.x * num2;
	ans.y = num1.y * num2;
	ans.z = num1.z * num2;

	return ans;
}

Quaternion operator*(const float num1, const Quaternion& num2) {
	Quaternion ans;
	ans.w = num1 * num2.w;
	ans.x = num1 * num2.x;
	ans.y = num1 * num2.y;
	ans.z = num1 * num2.z;

	return ans;
}

Quaternion operator+(const Quaternion& num1, const Quaternion& num2) {
	Quaternion ans;
	ans.w = num1.w + num2.w;
	ans.x = num1.x + num2.x;
	ans.y = num1.y + num2.y;
	ans.z = num1.z + num2.z;

	return ans;
}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	Quaternion ans;

	ans.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	ans.x = lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x;
	ans.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;
	ans.z = lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z;
	return ans;
}

Quaternion IdentityQuaternion(){
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion Conjugate(const Quaternion& quaternion){
	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

float Norm(const Quaternion& quaternion){
	return sqrtf(powf(quaternion.w, 2) + powf(quaternion.x, 2) + powf(quaternion.y, 2) + powf(quaternion.z, 2));
}

Quaternion Normalize(const Quaternion& quaternion){
	float norm = Norm(quaternion);
	Quaternion ans;
	ans.w = quaternion.w / norm;
	ans.x = quaternion.x / norm;
	ans.y = quaternion.y / norm;
	ans.z = quaternion.z / norm;
	return ans;
}

Quaternion Inverse(const Quaternion& quaternion){
	Quaternion conjugate = Conjugate(quaternion);
	float norm = Norm(quaternion);

	Quaternion ans;
	ans.w = conjugate.w / powf(norm, 2);
	ans.x = conjugate.x / powf(norm, 2);
	ans.y = conjugate.y / powf(norm, 2);
	ans.z = conjugate.z / powf(norm, 2);
	return ans;
}

float Dot(const Quaternion& q0, const Quaternion& q1) {
	return q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	Quaternion quaternion;

	quaternion.w = std::cos(angle / 2.0f);
	quaternion.x = axis.x * std::sin(angle / 2.0f);
	quaternion.y = axis.y * std::sin(angle / 2.0f);
	quaternion.z = axis.z * std::sin(angle / 2.0f);

	return quaternion;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	//共役
	Quaternion conjugate = Conjugate(quaternion);
	Quaternion r = {vector.x, vector.y, vector.z, 0};

	Quaternion rDash = Multiply(quaternion, Multiply(r, conjugate));

	return { rDash.x, rDash.y, rDash.z };
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {

	Matrix4x4 matrix = {};

	matrix.m[0][0] = powf(quaternion.w, 2) + powf(quaternion.x, 2) - powf(quaternion.y, 2) - powf(quaternion.z, 2);
	matrix.m[0][1] = 2 * (quaternion.x * quaternion.y + quaternion.w * quaternion.z);
	matrix.m[0][2] = 2 * (quaternion.x * quaternion.z - quaternion.w * quaternion.y);
	matrix.m[0][3] = 0;

	matrix.m[1][0] = 2 * (quaternion.x * quaternion.y - quaternion.w * quaternion.z);
	matrix.m[1][1] = powf(quaternion.w, 2) - powf(quaternion.x, 2) + powf(quaternion.y, 2) - powf(quaternion.z, 2);
	matrix.m[1][2] = 2 * (quaternion.y * quaternion.z + quaternion.w * quaternion.x);
	matrix.m[1][3] = 0;

	matrix.m[2][0] = 2 * (quaternion.x * quaternion.z + quaternion.w * quaternion.y);
	matrix.m[2][1] = 2 * (quaternion.y * quaternion.z - quaternion.w * quaternion.x);
	matrix.m[2][2] = powf(quaternion.w, 2) - powf(quaternion.x, 2) - powf(quaternion.y, 2) + powf(quaternion.z, 2);
	matrix.m[2][3] = 0;

	matrix.m[3][0] = 0;
	matrix.m[3][1] = 0;
	matrix.m[3][2] = 0;
	matrix.m[3][3] = 1;

	return matrix;
}

Quaternion Slerp(Quaternion q0, Quaternion q1, float t) {
	float dot = Dot(q0, q1);

	if (dot < 0) {
		q0 = { -q0.x, -q0.y, -q0.z, -q0.w };
		dot = -dot;
	}

	if (dot >= 1.0f - 0.0005f) {
		return (1.0f - t) * q0 + t * q1;
	}

	float theta = std::acos(dot);

	//
	float scale0 = std::sin((1.0f - t) * theta) / std::sin(theta);
	float scale1 = std::sin(t * theta) / std::sin(theta);
	
	return scale0 * q0 + scale1 * q1;
}