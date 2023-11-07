#include "WinApp.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_dx12.h"
#include "External/imgui/imgui_impl_win32.h"
#pragma comment(lib, "winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WinApp* WinApp::GetInstance() {
	static WinApp instance;
	return &instance;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	switch (msg){
		//ウィンドウが破棄されたとき
	case WM_DESTROY:
		//OSにアプリ終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

const wchar_t WinApp::className[] = L"MyEngine";
void WinApp::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t windowWidth, int32_t windowHeight) {

	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	//メンバ変数の初期化
	windowStyle_ = windowStyle;

	//ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc_.lpszClassName = className;
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//ウィンドウクラスを登録
	RegisterClass(&wc_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = {0, 0, kWindowWidth, kWindowHeight};
	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, windowStyle_, false);

	hwnd_ = CreateWindow(
		wc_.lpszClassName,
		title,
		windowStyle_,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc_.hInstance,
		nullptr
	);

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

void WinApp::DiscardingWindow() {

	//Comの終了処理
	CoUninitialize();

	//ウィンドウクラスの登録解除
	UnregisterClass(wc_.lpszClassName, wc_.hInstance);
}

bool WinApp::ProcessMessage() {
	MSG msg;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}