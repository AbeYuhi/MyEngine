#pragma once
#include <variant>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "Data/LevelData.h"
#include "External/nlohmann/json.hpp"

using json = nlohmann::json;

class LoadLevelData
{
public:
	static LoadLevelData* GetInstance();

private:
	LoadLevelData() = default;
	~LoadLevelData() = default;

	void LoadFile(std::string fileName);

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/ScriptFilles/";
};

