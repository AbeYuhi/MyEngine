#pragma once
#include "DirectXGame/DebugSytem/DebugLog.h"
#include "DirectXGame/DebugSytem/ReleseCheck.h"
#include "DirectXGame/LoadFile/GlobalVariables.h"
#include "DirectXGame/Base/SafeDelete/SafeDelete.h"
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Manager/TextureManager.h"
#include "DirectXGame/Manager/AudioManager.h"
#include "DirectXGame/Manager/GraphicsPipelineManager.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "DirectXGame/Manager/RandomManager.h"
#include "DirectXGame/Manager/InputManager.h"

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
	ImGuiManager* imGuiManager_ = nullptr;
	RandomManager* randomManager_ = nullptr;
	GraphicsPipelineManager* graphicsPipelineManager_ = nullptr;
	GlobalVariables* globalVariables_ = nullptr;
};

