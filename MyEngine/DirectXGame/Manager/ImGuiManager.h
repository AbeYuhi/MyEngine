#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_dx12.h"
#include "External/imgui/imgui_impl_win32.h"

#ifdef _DEBUG
using namespace ImGui;

class ImGuiManager
{
public:
	static ImGuiManager* GetInstance();

	void Initialize();

	void Begin();

	void End();

	void Draw();

	void ReleseProcess();

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;
};
#endif // _DEBUG