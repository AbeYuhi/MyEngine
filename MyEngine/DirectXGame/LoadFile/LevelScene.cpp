#include "LevelScene.h"

void LevelScene::Initialize(std::string fileName) {

	//ステージの読み込み
	LoadFile(fileName);

	//ステージの生成
	LevelCreate();

}

void LevelScene::Update() {

}

void LevelScene::Draw() {

	for (auto& levelObject : levelObjects) {
		levelObject.model_->Draw(levelObject.renderItem);
	}

}

void LevelScene::LoadFile(std::string fileName) {
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
	levelData_ = std::make_unique<LevelData>();

	//"objects"の全オブジェクトを走査
	for (auto& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理

		//Mesh
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData_->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData_->objects.back();

			if (object.contains("file_name")) {
				objectData.fileName = object["file_name"];
			}

			//トランスフォーム
			json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転角
			objectData.rotation.x = (float)transform["rotation"][0];
			objectData.rotation.y = (float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][1];
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			if (object.contains("collider")) {
				json& collider = object["collider"];

				objectData.collider = std::make_unique<LevelData::ObjectCollider>();
				//コライダータイプ
				objectData.collider->type = collider["type"];
				//ポジション
				objectData.collider->centerPos.x = collider["center"][0];
				objectData.collider->centerPos.y = collider["center"][2];
				objectData.collider->centerPos.z = collider["center"][1];
				//サイズ
				objectData.collider->size.x = collider["size"][0];
				objectData.collider->size.y = collider["size"][2];
				objectData.collider->size.z = collider["size"][1];
			}

			if (object.contains("children")) {
				ScanChildData(levelData_.get(), object, &objectData);
			}
		}

		//Light


		//Camera


	}
}

void LevelScene::ScanChildData(LevelData* levelData, json& childrens, LevelData::ObjectData* parent) {
	for (auto& object : childrens["children"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理

		//Mesh
		if (type.compare("MESH") == 0) {
			//要素追加
			parent->childrens.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = parent->childrens.back();

			if (object.contains("file_name")) {
				objectData.fileName = object["file_name"];
			}

			//トランスフォーム
			json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転角
			objectData.rotation.x = (float)transform["rotation"][0];
			objectData.rotation.y = (float)transform["rotation"][2];
			objectData.rotation.z = (float)transform["rotation"][1];
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			if (object.contains("collider")) {
				json& collider = object["collider"];

				objectData.collider = std::make_unique<LevelData::ObjectCollider>();
				//コライダータイプ
				objectData.collider->type = collider["type"];
				//ポジション
				objectData.collider->centerPos.x = collider["center"][0];
				objectData.collider->centerPos.y = collider["center"][2];
				objectData.collider->centerPos.z = collider["center"][1];
				//サイズ
				objectData.collider->size.x = collider["size"][0];
				objectData.collider->size.y = collider["size"][2];
				objectData.collider->size.z = collider["size"][1];
			}

			if (object.contains("children")) {
				ScanChildData(levelData, object["children"], &objectData);
			}
		}
	}
}

void LevelScene::LevelCreate() {

	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData_->objects) {
		LevelObject levelObject;
		levelObject.renderItem.Initialize();
		levelObject.renderItem.worldTransform_.data_.translate_ = objectData.translation;
		levelObject.renderItem.worldTransform_.data_.rotate_ = objectData.rotation;
		levelObject.renderItem.worldTransform_.data_.scale_ = objectData.scaling;
		levelObject.model_ = Model::Create(objectData.fileName);

		if (objectData.collider) {
			levelObject.collider.Initialize(&levelObject.renderItem.worldTransform_.data_.translate_, objectData.collider->size, WALL, false);
			CollisionManager::GetInstance()->AddCollider(&levelObject.collider);
		}

		for (auto& children : objectData.childrens) {
			LevelObject childrenObj;
			childrenObj.model_ = Model::Create(children.fileName);
			childrenObj.renderItem.Initialize();
			childrenObj.renderItem.worldTransform_.data_.translate_ = children.translation;
			childrenObj.renderItem.worldTransform_.data_.rotate_ = children.rotation;
			childrenObj.renderItem.worldTransform_.data_.scale_ = children.scaling;
			childrenObj.renderItem.worldTransform_.parent_ = &levelObject.renderItem.worldTransform_;
			if (children.collider) {
				childrenObj.collider.Initialize(&childrenObj.renderItem.worldTransform_.data_.translate_, childrenObj.renderItem.worldTransform_.data_.scale_, WALL, false);
				CollisionManager::GetInstance()->AddCollider(&childrenObj.collider);
			}
			levelObjects.push_back(std::move(childrenObj));
		}

		levelObjects.push_back(std::move(levelObject));
	}

}