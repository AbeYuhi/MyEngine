#include "Matrix4x4.h"
#include "Vector3_Math.hpp"

Matrix4x4 operator*(const Matrix4x4& num1, const Matrix4x4& num2) {
	return Multiply(num1, num2);
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

Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2) {
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

	return MakeRotateXYZMatrix(rotateXMatrix, rotateYMatrix, rotateZMatrix);
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate){
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
	float w  = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
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