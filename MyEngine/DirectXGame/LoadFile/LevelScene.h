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
#include "DirectXGame/Manager/CollisionManager.h"
#include "Object/Model.h"
#include "Data/LevelData.h"
#include "Data/Collider.h"

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

	void ScanChildData(LevelData* levelData, json& childrens, int32_t parent);

	void LevelCreate();

private: //メンバ変数
	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/ScriptFilles/";
	std::unique_ptr<LevelData> levelData_;

	struct LevelObject {
		std::string objName;
		std::shared_ptr<Model> model;
		RenderItem renderItem;
		Collider collider;
	};

	std::vector<std::unique_ptr<LevelObject>> levelObjects_;
};

