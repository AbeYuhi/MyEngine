#include "Vector3.h"

Vector3 operator+(const Vector3& num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1.x + num2.x;
	num3.y = num1.y + num2.y;
	num3.z = num1.z + num2.z;

	return num3;
}

Vector3 operator-(const Vector3& num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1.x - num2.x;
	num3.y = num1.y - num2.y;
	num3.z = num1.z - num2.z;

	return num3;
}

Vector3 operator*(const Vector3& num1, const float num2) {
	Vector3 num3;

	num3.x = num1.x * num2;
	num3.y = num1.y * num2;
	num3.z = num1.z * num2;

	return num3;
}

Vector3 operator*(const float num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1 * num2.x;
	num3.y = num1 * num2.y;
	num3.z = num1 * num2.z;

	return num3;
}

Vector3 operator*(const Vector3& num1, const Vector3& num2) {
	Vector3 num3;

	num3.x = num1.x * num2.x;
	num3.y = num1.y * num2.y;
	num3.z = num1.z * num2.z;

	return num3;
}

Vector3 operator/(const Vector3& num1, const float num2) {
	Vector3 num3;

	num3.x = num1.x * num2;
	num3.y = num1.y * num2;
	num3.z = num1.z * num2;

	return num3;
}

Vector3 operator+=(Vector3& num1, const Vector3& num2) {

	num1.x += num2.x;
	num1.y += num2.y;
	num1.z += num2.z;

	return num1;
}

Vector3 operator-=(Vector3& num1, const Vector3& num2) {

	num1.x -= num2.x;
	num1.y -= num2.y;
	num1.z -= num2.z;

	return num1;
}

Vector3 operator*=(Vector3& num1, const float num2) {

	num1.x *= num2;
	num1.y *= num2;
	num1.z *= num2;

	return num1;
}

Vector3 operator*=(Vector3& num1, const Vector3& num2) {

	num1.x *= num2.x;
	num1.y *= num2.y;
	num1.z *= num2.z;

	return num1;
}

Vector3 operator/=(Vector3& num1, const Vector3& num2) {

	num1.x /= num2.x;
	num1.y /= num2.y;
	num1.z /= num2.z;

	return num1;
}

Vector3 operator/=(Vector3& num1, const float num2) {

	num1.x = num1.x * num2;
	num1.y = num1.y * num2;
	num1.z = num1.z * num2;

	return num1;
}

bool operator==(const Vector3& num1, const Vector3& num2) {
	if (num1.x == num2.x
		&& num1.y == num2.y
		&& num1.z == num2.z) {
		return true;
	}

	return false;
}