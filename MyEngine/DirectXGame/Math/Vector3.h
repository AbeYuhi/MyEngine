#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

struct Line {
	Vector3 origin; //始点
	Vector3 diff;   //終点への差分ベクトル
};

struct Ray {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};

struct Segment {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};

//グローバル算術演算子のオーバーロード
Vector3 operator+(const Vector3& num1, const Vector3& num2);
Vector3 operator-(const Vector3& num1, const Vector3& num2);
Vector3 operator*(const Vector3& num1, const float num2);
Vector3 operator*(const float num1, const Vector3& num2);
Vector3 operator*(const Vector3& num1, const Vector3& num2);
Vector3 operator/(const Vector3& num1, const float num2);
Vector3 operator+=(Vector3& num1, const Vector3& num2);
Vector3 operator-=(Vector3& num1, const Vector3& num2);
Vector3 operator*=(Vector3& num1, const float num2);
Vector3 operator*=(Vector3& num1, const Vector3& num2);
Vector3 operator/=(Vector3& num1, const Vector3& num2);
Vector3 operator/=(Vector3& num1, const float num2);
bool operator==(const Vector3& num1, const Vector3& num2);