#pragma once
#include "DebugSytem/DebugLog.h"
#include "DebugSytem/ReleseCheck.h"
#include "LoadFile/GlobalVariables.h"
#include "Base/SafeDelete/SafeDelete.h"
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Manager/TextureManager.h"
#include "Manager/AudioManager.h"
#include "Manager/GraphicsPipelineManager.h"
#include "Manager/PostEffectManager.h"
#include "Manager/ImGuiManager.h"
#include "Manager/RandomManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/InputManager.h"
#include "Manager/LightObjectManager.h"

class Framework
{
public:
	Framework() = default;
	virtual ~Framework() = default;

	virtual void Initialize();

	virtual void Finalize();

protected:
	//汎用機能
	WinApp* winApp_ = nullptr;
	DirectXCommon* directXCommon_ = nullptr;
	InputManager* inputManager_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	AudioManager* audioManager_ = nullptr;
#ifdef _DEBUG
	ImGuiManager* imGuiManager_ = nullptr;
#endif //_DEBUG
	RandomManager* randomManager_ = nullptr;
	CollisionManager* collisionManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	PostEffectManager* postEffectManager_ = nullptr;
	GlobalVariables* globalVariables_ = nullptr;
	LightObjectManager* lightObjectManager_ = nullptr;
};

