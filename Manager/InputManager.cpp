#include "InputManager.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib, "xinput.lib")

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
	//マウスデバイスの生成
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard); //標準形式
	assert(SUCCEEDED(result));
	result = mouse_->SetDataFormat(&c_dfDIMouse); //標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
	result = mouse_->SetCooperativeLevel(
		winApp->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void InputManager::Update() {
	//キーボード情報の取得開始
	keyboard_->Acquire();
	//マウス情報の取得開始
	mouse_->Acquire();

	//すべてのキーの入力状態を取得する
	memcpy(preKeys_, keys_, sizeof(keys_));
	keyboard_->GetDeviceState(sizeof(keys_), keys_);

	//マウスの入力状態を取得
	memcpy(&preMouseState_, &mouseState_, sizeof(mouseState_));
	mouse_->GetDeviceState(sizeof(mouseState_), &mouseState_);
	
	preClientMousePos_ = clientMousePos_;
	POINT screenCursorPos;
	if (GetCursorPos(&screenCursorPos) && ScreenToClient(WinApp::GetInstance()->GetHWND(), &screenCursorPos)) {
		clientMousePos_ = { static_cast<float>(screenCursorPos.x), static_cast<float>(screenCursorPos.y) };
	}

	//コントローラーの接続確認
	DWORD dwResult;
	ZeroMemory(&preGamePadState_, sizeof(XINPUT_STATE));
	memcpy(&preGamePadState_, &gamePadState_, sizeof(gamePadState_));
	ZeroMemory(&gamePadState_, sizeof(XINPUT_STATE));
	dwResult = XInputGetState(0, &gamePadState_);

	if (dwResult == ERROR_SUCCESS) {
		isConnection_ = true;
	}
	else {
		isConnection_ = false;
	}
}