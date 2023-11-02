#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <WRL/client.h>
#include "../Base/WinApp/WinApp.h"
#include "../Base/DirectXCommon/DirectXCommon.h"
#include "../Math/Vector2.h"

using namespace Microsoft::WRL;

class InputManager {
public: //静的メンバ関数
	static InputManager* GetInstance();

public: //メンバ関数

	void Initialize();

	void Update();

	inline bool IsPushKey(BYTE keyNum) {
		if (keys_[keyNum]) {
			return true;
		}
		return false;
	}
	inline bool IsTriggerKey(BYTE keyNum) {
		if (keys_[keyNum] && !preKeys_[keyNum]) {
			return true;
		}
		return false;
	}
	inline bool IsReleaseKey(BYTE keyNum) {
		if (!keys_[keyNum] && preKeys_[keyNum]) {
			return true;
		}
		return false;
	}

	/// <summary>
	/// 左ボタン[0]
	/// 右ボタン[1]
	/// ホイール[2]
	/// サイドボタン[3]
	/// </summary>
	/// <param name="clickNum"></param>
	/// <returns></returns>
	inline bool IsMousePush(BYTE clickNum) {
		if (mouseState_.rgbButtons[clickNum]) {
			return true;
		}
		return false;
	}
	inline bool IsMouseTrigger(BYTE clickNum) {
		if (mouseState_.rgbButtons[clickNum] && !preMouseState_.rgbButtons[clickNum]) {
			return true;
		}
		return false;
	}
	inline bool IsMouseRelsease(BYTE clickNum) {
		if (!mouseState_.rgbButtons[clickNum] && preMouseState_.rgbButtons[clickNum]) {
			return true;
		}
		return false;
	}
	inline float GetMouseWheelDelta() {
		return static_cast<float>(mouseState_.lZ);
	}
	inline Vector2 GetMousePos() {
		return clientMousePos_;
	}
	inline Vector2 GetMouseMovement() {
		return clientMousePos_ - preClientMousePos_;
	}


private: //メンバ関数
	InputManager() = default;
	~InputManager() = default;
private: //メンバ変数
	ComPtr<IDirectInput8> directInput_;
	ComPtr<IDirectInputDevice8> keyboard_;
	ComPtr<IDirectInputDevice8> mouse_;

	//キーボード情報の格納変数
	BYTE keys_[256];
	BYTE preKeys_[256];
	//マウス情報の格納変数
	DIMOUSESTATE mouseState_;
	DIMOUSESTATE preMouseState_;
	Vector2 clientMousePos_;
	Vector2 preClientMousePos_;
};