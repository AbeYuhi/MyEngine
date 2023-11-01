#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <WRL/client.h>
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"

using namespace Microsoft::WRL;

class InputManager {
public: //静的メンバ関数
	static InputManager* GetInstance();

public: //メンバ関数

	void Initialize();

	void Update();

	inline bool IsPushKey(BYTE keyNum) {
		if (keys[keyNum]) {
			return true;
		}
		else {
			return false;
		}
	}
	inline bool IsTriggerKey(BYTE keyNum) {
		if (keys[keyNum] && !preKeys[keyNum]) {
			return true;
		}
		else {
			return false;
		}
	}
	inline bool IsReleaseKey(BYTE keyNum) {
		if (!keys[keyNum] && preKeys[keyNum]) {
			return true;
		}
		else {
			return false;
		}
	}

private: //メンバ関数
	InputManager() = default;
	~InputManager() = default;
private: //メンバ変数
	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> keyboard_;

	//キーボード情報の格納変数
	BYTE keys[256];
	BYTE preKeys[256];
};