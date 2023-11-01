#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp
{
public: //静的関数
	//インスタンスの取得
	static WinApp* GetInstance();

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: //静的メンバ変数
	static const wchar_t className[];
	static const int32_t kWindowWidth = 1280;
	static const int32_t kWindowHeight = 720;

public: //メンバ関数

	void CreateGameWindow(const wchar_t* title = L"MyEngine", UINT windowStyle = WS_OVERLAPPEDWINDOW, int32_t windowWidth = kWindowWidth, int32_t windowHeight = kWindowHeight);

	void DiscardingWindow();

	bool ProcessMessage();

	inline HWND GetHWND() { return hwnd_; }
	inline WNDCLASS GetWNDCLASS() { return wc_; }

private: //メンバ関数
	WinApp() = default;
	~WinApp() = default;

private: //メンバ変数
	HWND hwnd_;
	UINT windowStyle_;
	WNDCLASS wc_;
};