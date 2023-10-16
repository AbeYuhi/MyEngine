#pragma once
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_impl_dx12.h"
#include "../../External/imgui/imgui_impl_win32.h"
using namespace ImGui;

class ImGuiManager
{
public:
	static ImGuiManager* GetInstance();

	void Initialize();

	void Begin();

	void End();

	void Draw();

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;
};