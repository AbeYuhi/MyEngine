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

class LevelScene
{
public:
	LevelScene() = default;
	~LevelScene() = default;

	void Initialize(std::string fileName);

	void Update();

	void Draw();

private: //メンバ関数

	void LoadFile(std::string fileName);

	void ScanChildData(LevelData* levelData, json childrens, LevelData::ObjectData* parent);

	void LevelCreate();

private: //メンバ変数
	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/ScriptFilles/";
	std::unique_ptr<LevelData> levelData_;

	std::shared_ptr<Model> model_;
	std::list<RenderItem> renderItems_;
};

