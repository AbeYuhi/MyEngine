#include "LoadLevelData.h"

LoadLevelData* LoadLevelData::GetInstance() {
	static LoadLevelData instance;
	return &instance;
}

void LoadLevelData::LoadFile(std::string fileName) {
	const std::string fullPath = kDirectoryPath + fileName;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullPath);

	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	json deserialized;
	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンス
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();

	//"objects"の全オブジェクトを走査
	for (auto& object : deserialized["objects"]) {
		assert(object.contains("type"));
		
		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		
		//Mesh
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				objectData.fileName = object["file_name"];
			}

			//トランスフォーム

		}
	}

}