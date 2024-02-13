#pragma once
#include "DirectXGame/Data/Transform.h"
#include "DirectXGame/Math/Vector2.h"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Math/Matrix4x4.h"
#include "DirectXGame/Math/Math.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/InputManager.h"
#define M_PI 3.14f

class SpriteCamera
{
public:
	static SpriteCamera* GetInstance();

	void Initialize();

	void Update();

	inline TransformData GetWorldTransrom() { return transform_; }
	inline Matrix4x4 GetViewMatrix() { return viewMatrix_; }
	inline Matrix4x4 GetProjectionMatrix() { return projectionMatrix_; }
	inline Matrix4x4 GetViewProjectionMatrix() { return viewProjectionMatrix_; }
	inline const Matrix4x4* GetPViewProjectionMatrix() { return &viewProjectionMatrix_; }
	inline float GetNearClip() { return nearClip_; }
	inline float GetFarClip() { return farClip_; }

private:
	SpriteCamera() = default;
	~SpriteCamera() = default;

	TransformData transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	float nearClip_;
	float farClip_;
	bool isMove_;
};