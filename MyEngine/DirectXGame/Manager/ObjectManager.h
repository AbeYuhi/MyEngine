#pragma once
#include <variant>
#include <vector>
#include <map>
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include "DirectXGame/Base/CreateResource/CreateResource.h"
#include "DirectXGame/Object/Model.h"
#include "DirectXGame/Object/Sprite.h"
#include "DirectXGame/Object/WireFrameBox.h"


class ObjectManager
{
public:
	//インスタンスの取得
	static ObjectManager* GetInstance();
	static UINT sModelNum_;

	/// <summary>
	/// ゲーム上に描画する3Dモデルの作成
	/// </summary>
	/// <param name="fileName">読み込みたいObjファイル名</param>
	/// <returns></returns>
	static Model* Create(const std::string& fileName = "cube");
	static WireFrameBox* CreateWireFrameBox();
	static Sprite* CreateSprite();

private:

	Model* LoadObj(const std::string& fileName);
	WireFrameBox* LoadWireFrameBox();
	Sprite* LoadSprite();

private:
	ObjectManager() = default;
	~ObjectManager() = default;

	std::map<std::string, std::unique_ptr<Model>> modelDatas_;
	std::unique_ptr<WireFrameBox> wireFrameBoxData_;
	std::unique_ptr<Sprite> spriteData_;
};
