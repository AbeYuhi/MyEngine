#pragma once
#include "../Math/Transform.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector3_Math.hpp"
#include "../Math/Matrix4x4.h"
#include "../Base/WinApp/WinApp.h"
#define M_PI 3.14f

class DebugCamera
{
public:
	DebugCamera();
	~DebugCamera();

	void Initialize();

	void Update();

	inline WorldTransform GetWorldTransrom() { return transform_; }
	inline Matrix4x4 GetViewMatrix() { return viewMatrix_; }
	inline Matrix4x4 GetProjectionMatrix() { return projectionMatrix_; }
	inline Matrix4x4 GetViewProjectionMatrix() { return viewProjectionMatrix_; }
	inline float GetNearClip() { return nearClip_; }
	inline float GetFarClip() { return farClip_; }
	inline float GetFovY() { return fovY_; }

private:
	
	WorldTransform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	float nearClip_;
	float farClip_;
	float fovY_;

	//マウスの位置
	Vector2Int mousePos_;
	Vector2Int preMousePos_;
};
