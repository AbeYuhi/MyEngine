#pragma once
#include <variant>
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include "External/nlohmann/json.hpp"
#include "DirectXGame/Math/Vector3.h"
#include "DirectXGame/Manager/ImGuiManager.h"
#include "Object/Model.h"
#include "Data/LevelData.h"

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

	struct LevelObject {
		std::shared_ptr<Model> model_;
		RenderItem renderItem;
	};

	std::list<LevelObject> levelObjects;
};

