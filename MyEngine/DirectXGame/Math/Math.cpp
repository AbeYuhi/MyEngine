#include "Math.h"

#pragma region Vector3
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = {0, 0, 0};

	v3.x = v1.x + v2.x;

	v3.y = v1.y + v2.y;
	
	v3.z = v1.z + v2.z;

	return v3;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = v1.x - v2.x;

	v3.y = v1.y - v2.y;

	v3.z = v1.z - v2.z;

	return v3;
}

Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = scalar * v.x;
 
	v3.y = scalar * v.y;
 
	v3.z = scalar * v.z;

	return v3;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	float dot = 0.0f;

	dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return dot;
}

float Length(const Vector3& v) {
	float length = 0;

	length = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));

	return length;
}

Vector3 Normalize(const Vector3& v1) {
	Vector3 v2 = { 0, 0, 0 };
	float length = Length(v1);

	v2.x = v1.x / length;
	v2.y = v1.y / length;
	v2.z = v1.z / length;

	return v2;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 v3{};
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;

	return v3;
}

bool IsFront(const Vector3& v1, const Vector3 obj[3]) {

	Vector3 vecA = obj[1] - obj[0];
	Vector3 vecB = obj[2] - obj[1];

	Vector3 v2 = Cross(vecA, vecB);

	float dot = Dot(v1, v2);

	if (dot <= 0) {
		return true;
	}
	return false;
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float scalar = Dot(v1, Normalize(v2));

	return Multiply(scalar, Normalize(v2));
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 cp = segment.origin + project;
	return cp;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};
	return result;
}

Vector3 Perpendicular(const Vector3& v) {
	if (v.x != 0.0f || v.y != 0.0f) {
		return{ -v.y, v.x, 0.0f };
	}
	return{ 0.0f, -v.z, v.y };
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 v3 = {};

	v3.x = v1.x + t * (v2.x - v1.x);
	v3.y = v1.y + t * (v2.y - v1.y);
	v3.z = v1.z + t * (v2.z - v1.z);

	return v3;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 v3 = {};

	if (v1 == v2) {
		v3 = Lerp(v1, v2, t);
		return v3;
	}
	else {
		// 2ベクトル間の角度（鋭角側）
		float angle = std::acos(Dot(v1, v2));

		// sinθ
		float SinTh = std::sin(angle);

		// 補間係数
		float Ps = std::sin(angle * (1 - t));
		float Pe = std::sin(angle * t);

		v3.x = (Ps * v1.x + Pe * v2.x) / SinTh;
		v3.y = (Ps * v1.y + Pe * v2.y) / SinTh;
		v3.z = (Ps * v1.z + Pe * v2.z) / SinTh;

		// 一応正規化して球面線形補間に
		Normalize(v3);

		return v3;
	}
}

#pragma endregion

#pragma region Matrix4x4

Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2) {
	return Multiply(num1, num2);
}

Matrix4x4 operator*=(Matrix4x4& num1, const Matrix4x4& num2) {
	num1 = Multiply(num1, num2);
	return num1;
}

bool operator==(const Matrix4x4& num1, const Matrix4x4& num2) {
	bool orTF = true;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (num1.m[y][x] != num2.m[y][x]) {
				orTF = false;
			}
		}
	}
	return orTF;
}

Matrix4x4 Add(Matrix4x4 matrix1, Matrix4x4 matrix2) {
	Matrix4x4 matrix = {};
	for (int row = 0; row < 4; row++) {
		for (int colmun = 0; colmun < 4; colmun++) {
			matrix.m[row][colmun] = matrix1.m[row][colmun] + matrix2.m[row][colmun];
		}
	}
	return matrix;
}

Matrix4x4 Subtract(Matrix4x4 matrix1, Matrix4x4 matrix2) {
	Matrix4x4 matrix = {};
	for (int row = 0; row < 4; row++) {
		for (int colmun = 0; colmun < 4; colmun++) {
			matrix.m[row][colmun] = matrix1.m[row][colmun] - matrix2.m[row][colmun];
		}
	}
	return matrix;
}

Matrix4x4 Multiply(const Matrix4x4 matrix1, const Matrix4x4 matrix2) {
	Matrix4x4 matrix = {};

	matrix.m[0][0] = matrix1.m[0][0] * matrix2.m[0][0] + matrix1.m[0][1] * matrix2.m[1][0] + matrix1.m[0][2] * matrix2.m[2][0] + matrix1.m[0][3] * matrix2.m[3][0];
	matrix.m[0][1] = matrix1.m[0][0] * matrix2.m[0][1] + matrix1.m[0][1] * matrix2.m[1][1] + matrix1.m[0][2] * matrix2.m[2][1] + matrix1.m[0][3] * matrix2.m[3][1];
	matrix.m[0][2] = matrix1.m[0][0] * matrix2.m[0][2] + matrix1.m[0][1] * matrix2.m[1][2] + matrix1.m[0][2] * matrix2.m[2][2] + matrix1.m[0][3] * matrix2.m[3][2];
	matrix.m[0][3] = matrix1.m[0][0] * matrix2.m[0][3] + matrix1.m[0][1] * matrix2.m[1][3] + matrix1.m[0][2] * matrix2.m[2][3] + matrix1.m[0][3] * matrix2.m[3][3];

	matrix.m[1][0] = matrix1.m[1][0] * matrix2.m[0][0] + matrix1.m[1][1] * matrix2.m[1][0] + matrix1.m[1][2] * matrix2.m[2][0] + matrix1.m[1][3] * matrix2.m[3][0];
	matrix.m[1][1] = matrix1.m[1][0] * matrix2.m[0][1] + matrix1.m[1][1] * matrix2.m[1][1] + matrix1.m[1][2] * matrix2.m[2][1] + matrix1.m[1][3] * matrix2.m[3][1];
	matrix.m[1][2] = matrix1.m[1][0] * matrix2.m[0][2] + matrix1.m[1][1] * matrix2.m[1][2] + matrix1.m[1][2] * matrix2.m[2][2] + matrix1.m[1][3] * matrix2.m[3][2];
	matrix.m[1][3] = matrix1.m[1][0] * matrix2.m[0][3] + matrix1.m[1][1] * matrix2.m[1][3] + matrix1.m[1][2] * matrix2.m[2][3] + matrix1.m[1][3] * matrix2.m[3][3];

	matrix.m[2][0] = matrix1.m[2][0] * matrix2.m[0][0] + matrix1.m[2][1] * matrix2.m[1][0] + matrix1.m[2][2] * matrix2.m[2][0] + matrix1.m[2][3] * matrix2.m[3][0];
	matrix.m[2][1] = matrix1.m[2][0] * matrix2.m[0][1] + matrix1.m[2][1] * matrix2.m[1][1] + matrix1.m[2][2] * matrix2.m[2][1] + matrix1.m[2][3] * matrix2.m[3][1];
	matrix.m[2][2] = matrix1.m[2][0] * matrix2.m[0][2] + matrix1.m[2][1] * matrix2.m[1][2] + matrix1.m[2][2] * matrix2.m[2][2] + matrix1.m[2][3] * matrix2.m[3][2];
	matrix.m[2][3] = matrix1.m[2][0] * matrix2.m[0][3] + matrix1.m[2][1] * matrix2.m[1][3] + matrix1.m[2][2] * matrix2.m[2][3] + matrix1.m[2][3] * matrix2.m[3][3];

	matrix.m[3][0] = matrix1.m[3][0] * matrix2.m[0][0] + matrix1.m[3][1] * matrix2.m[1][0] + matrix1.m[3][2] * matrix2.m[2][0] + matrix1.m[3][3] * matrix2.m[3][0];
	matrix.m[3][1] = matrix1.m[3][0] * matrix2.m[0][1] + matrix1.m[3][1] * matrix2.m[1][1] + matrix1.m[3][2] * matrix2.m[2][1] + matrix1.m[3][3] * matrix2.m[3][1];
	matrix.m[3][2] = matrix1.m[3][0] * matrix2.m[0][2] + matrix1.m[3][1] * matrix2.m[1][2] + matrix1.m[3][2] * matrix2.m[2][2] + matrix1.m[3][3] * matrix2.m[3][2];
	matrix.m[3][3] = matrix1.m[3][0] * matrix2.m[0][3] + matrix1.m[3][1] * matrix2.m[1][3] + matrix1.m[3][2] * matrix2.m[2][3] + matrix1.m[3][3] * matrix2.m[3][3];

	return matrix;
}

float Det(Matrix4x4 matrix) {
	float det = 0.0f;

	det = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3]
		+ matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1]
		+ matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]

		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1]
		- matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3]
		- matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3]
		- matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]
		- matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]

		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3]
		+ matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]

		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3]
		+ matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1]
		+ matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3]
		- matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]

		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0]
		- matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0]
		- matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]

		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0]
		+ matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0]
		+ matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];

	return det;
}

Matrix4x4 Inverse(Matrix4x4 matrix) {
	Matrix4x4 matrix1 = {};
	float det = Det(matrix);

	matrix1.m[0][0] = (1.0f / det) * (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3]
		+ matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1]
		+ matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]
		- matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1]
		- matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3]
		- matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[0][1] = (1.0f / det) * (-1 * matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3]
		- matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1]
		- matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2]
		+ matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1]
		+ matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3]
		+ matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[0][2] = (1.0f / det) * (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3]
		+ matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1]
		+ matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2]
		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3]
		- matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]);
	matrix1.m[0][3] = (1.0f / det) * (-1 * matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3]
		- matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1]
		- matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2]
		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1]
		+ matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3]
		+ matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]);

	matrix1.m[1][0] = (1.0f / det) * (-1 * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3]
		- matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0]
		- matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]
		+ matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0]
		+ matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3]
		+ matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[1][1] = (1.0f / det) * (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3]
		+ matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0]
		+ matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2]
		- matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0]
		- matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3]
		- matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]);
	matrix1.m[1][2] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3]
		- matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0]
		- matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2]
		+ matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3]
		+ matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]);
	matrix1.m[1][3] = (1.0f / det) * (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3]
		+ matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0]
		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2]
		- matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0]
		- matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3]
		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]);

	matrix1.m[2][0] = (1.0f / det) * (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3]
		+ matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0]
		+ matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1]
		- matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0]
		- matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3]
		- matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]);
	matrix1.m[2][1] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3]
		- matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0]
		- matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1]
		+ matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0]
		+ matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3]
		+ matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]);
	matrix1.m[2][2] = (1.0f / det) * (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3]
		+ matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0]
		+ matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1]
		- matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0]
		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3]
		- matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]);
	matrix1.m[2][3] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3]
		- matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0]
		- matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1]
		+ matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0]
		+ matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3]
		+ matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]);

	matrix1.m[3][0] = (1.0f / det) * (-1 * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]
		- matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]
		- matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1]
		+ matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0]
		+ matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]
		+ matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]);
	matrix1.m[3][1] = (1.0f / det) * (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2]
		+ matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0]
		+ matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1]
		- matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0]
		- matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2]
		- matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]);
	matrix1.m[3][2] = (1.0f / det) * (-1 * matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2]
		- matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0]
		- matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1]
		+ matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0]
		+ matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2]
		+ matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]);
	matrix1.m[3][3] = (1.0f / det) * (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2]
		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0]
		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2]
		- matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]);

	return matrix1;
}

Matrix4x4 Transpose(Matrix4x4 matrix) {
	Matrix4x4 matrix1 = {};

	matrix1.m[0][0] = matrix.m[0][0];
	matrix1.m[0][1] = matrix.m[1][0];
	matrix1.m[0][2] = matrix.m[2][0];
	matrix1.m[0][3] = matrix.m[3][0];

	matrix1.m[1][0] = matrix.m[0][1];
	matrix1.m[1][1] = matrix.m[1][1];
	matrix1.m[1][2] = matrix.m[2][1];
	matrix1.m[1][3] = matrix.m[3][1];

	matrix1.m[2][0] = matrix.m[0][2];
	matrix1.m[2][1] = matrix.m[1][2];
	matrix1.m[2][2] = matrix.m[2][2];
	matrix1.m[2][3] = matrix.m[3][2];

	matrix1.m[3][0] = matrix.m[0][3];
	matrix1.m[3][1] = matrix.m[1][3];
	matrix1.m[3][2] = matrix.m[2][3];
	matrix1.m[3][3] = matrix.m[3][3];

	return matrix1;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 matrix = {};

	matrix.m[0][0] = 1.0f;
	matrix.m[0][1] = 0.0f;
	matrix.m[0][2] = 0.0f;
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 0.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[1][2] = 0.0f;
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 0.0f;
	matrix.m[2][1] = 0.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[1][1] = std::cos(radian);
	matrix.m[1][2] = std::sin(radian);

	matrix.m[2][1] = std::sin(radian) * -1;
	matrix.m[2][2] = std::cos(radian);

	return matrix;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = std::cos(radian);
	matrix.m[0][2] = std::sin(radian) * -1;

	matrix.m[2][0] = std::sin(radian);
	matrix.m[2][2] = std::cos(radian);

	return matrix;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = std::cos(radian);
	matrix.m[0][1] = std::sin(radian);

	matrix.m[1][0] = std::sin(radian) * -1;
	matrix.m[1][1] = std::cos(radian);

	return matrix;
}

Matrix4x4 MakeRotateXYZMatrix(const Matrix4x4& matrixX, const Matrix4x4& matrixY, const Matrix4x4& matrixZ) {
	return Multiply(matrixX, Multiply(matrixY, matrixZ));
}

Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	return MakeRotateXYZMatrix(rotateXMatrix, rotateZMatrix, rotateYMatrix);
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 matrix = MakeIdentity4x4();
	matrix.m[3][0] = translate.x;
	matrix.m[3][1] = translate.y;
	matrix.m[3][2] = translate.z;

	return matrix;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 matrix = {};

	matrix.m[0][0] = scale.x;
	matrix.m[1][1] = scale.y;
	matrix.m[2][2] = scale.z;
	matrix.m[3][3] = 1;

	return matrix;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = {};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakeAffineMatrix(const EulerTransformData& transformData) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(transformData.scale_);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(transformData.rotate_);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(transformData.translate_);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakeAffineMatrix(const QuaternionTransformData& transformData) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(transformData.scale_);
	Matrix4x4 rotateXYZMatrix = MakeRotateMatrix(transformData.rotate_);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(transformData.translate_);

	return Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 matrix = { 0 };

	matrix.m[0][0] = (1.0f / aspectRatio) * (1.0f / tanf(fovY / 2.0f));
	matrix.m[1][1] = (1.0f / tanf(fovY / 2.0f));
	matrix.m[2][2] = farClip / (farClip - nearClip);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][2] = (-1.0f * nearClip * farClip) / (farClip - nearClip);


	return matrix;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = 2.0f / (right - left);
	matrix.m[1][1] = 2.0f / (top - bottom);
	matrix.m[2][2] = 1.0f / (farClip - nearClip);

	matrix.m[3][0] = (left + right) / (left - right);
	matrix.m[3][1] = (top + bottom) / (bottom - top);
	matrix.m[3][2] = (nearClip) / (nearClip - farClip);

	return matrix;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 matrix = MakeIdentity4x4();

	matrix.m[0][0] = width / 2.0f;
	matrix.m[1][1] = -1 * (height / 2.0f);
	matrix.m[2][2] = maxDepth - minDepth;

	matrix.m[3][0] = left + width / 2.0f;
	matrix.m[3][1] = top + height / 2.0f;
	matrix.m[3][2] = minDepth;

	return matrix;
}
#pragma endregion

void ControlMinMax(AABB& a) {
	a.min.x = (std::min)(a.min.x, a.max.x);
	a.max.x = (std::max)(a.min.x, a.max.x);

	a.min.y = (std::min)(a.min.y, a.max.y);
	a.max.y = (std::max)(a.min.y, a.max.y);

	a.min.z = (std::min)(a.min.z, a.max.z);
	a.max.z = (std::max)(a.min.z, a.max.z);
}

bool IsCollision(const AABB& a, const AABB& b) {
	if (a.min.x <= b.max.x && a.max.x >= b.min.x &&
		a.min.y <= b.max.y && a.max.y >= b.min.y &&
		a.min.z <= b.max.z && a.max.z >= b.min.z) {
		return true;
	}
	return false;
}	

bool IsCollision(const AABB& aabb, const Vector3& point) {
	if (aabb.min.x <= point.x && aabb.max.x >= point.x &&
		aabb.min.y <= point.y && aabb.max.y >= point.y &&
		aabb.min.z <= point.z && aabb.max.z >= point.z) {
		return true;
	}
	return false;
}

bool CalculateNormal(const AABB& a, const AABB& b) {

	Vector3 aCenter = { a.max.x + a.min.x / 2.0f, a.max.y + a.min.y / 2.0f , a.max.z + a.min.z / 2.0f };
	Vector3 bCenter = { b.max.x + b.min.x / 2.0f, b.max.y + b.min.y / 2.0f , b.max.z + b.min.z / 2.0f };
	Vector3 d = bCenter - aCenter;

}

bool CalculateNormal(const OBB& a, const OBB& b) {

}

bool CalculateNormal(const Sphere& a, const Sphere& b) {

}

EulerTransformData ExtractTransform(const Matrix4x4& matrix) {
	EulerTransformData data;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	// 位置の抽出
	position.x = matrix.m[3][0];
	position.y = matrix.m[3][1];
	position.z = matrix.m[3][2];

	// スケールの抽出
	scale.x = std::sqrt(matrix.m[0][0] * matrix.m[0][0] + matrix.m[1][0] * matrix.m[1][0] + matrix.m[2][0] * matrix.m[2][0]);
	scale.y = std::sqrt(matrix.m[0][1] * matrix.m[0][1] + matrix.m[1][1] * matrix.m[1][1] + matrix.m[2][1] * matrix.m[2][1]);
	scale.z = std::sqrt(matrix.m[0][2] * matrix.m[0][2] + matrix.m[1][2] * matrix.m[1][2] + matrix.m[2][2] * matrix.m[2][2]);

	// スケールを除去して回転成分を取り出す
	Matrix4x4 rotationMatrix = matrix;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i == 0) {
				rotationMatrix.m[i][j] /= scale.x;
			}
			else if (i == 1) {
				rotationMatrix.m[i][j] /= scale.y;
			}
			else {
				rotationMatrix.m[i][j] /= scale.z;
			}
		}
	}

	// 回転行列からオイラー角を計算する
	rotation.y = std::asin(-rotationMatrix.m[0][2]);
	if (std::cos(rotation.y) > 0.0001) {
		rotation.x = std::atan2(rotationMatrix.m[1][2], rotationMatrix.m[2][2]);
		rotation.z = std::atan2(rotationMatrix.m[0][1], rotationMatrix.m[0][0]);
	}
	else {
		rotation.x = std::atan2(-rotationMatrix.m[2][1], rotationMatrix.m[1][1]);
		rotation.z = 0;
	}

	data.translate_ = position;
	data.rotate_ = rotation;
	data.scale_ = scale;

	return data;
}
