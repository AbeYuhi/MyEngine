#include "InputManager.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

InputManager* InputManager::GetInstance() {
	static InputManager instance;
	return &instance;
}

void InputManager::Initialize() {
	//インスタンスの取得
	WinApp* winApp = WinApp::GetInstance();

	//DirectInputの初期化
	HRESULT result = DirectInput8Create(
		winApp->GetWNDCLASS().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard); //標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void InputManager::Update() {
	//キーボード情報の取得開始
	keyboard_->Acquire();

	//すべてのキーの入力状態を取得する
	memcpy(preKeys, keys, sizeof(keys));
	keyboard_->GetDeviceState(sizeof(keys), keys);
}