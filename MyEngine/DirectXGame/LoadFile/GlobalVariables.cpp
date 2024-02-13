#include "GlobalVariables.h"

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName) {
	dates_[groupName];
}

void GlobalVariables::Updates() {
#ifdef _DEBUG
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) {
		return;
	}

	//各グループについて
	for (std::map<std::string, Group>::iterator itGroup = dates_.begin(); itGroup != dates_.end(); ++itGroup) {
		//グループ名を取得
		const std::string& groupName = itGroup->first;
		//グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}

		//各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {
			//各項目を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}
			if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, -10.0f, 10.0f);
			}
			if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}

		//セーブ処理
		ImGui::Text("\n");
		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
#endif // _DEBUG
}

void GlobalVariables::SaveFile(const std::string& groupName) {
	//グループを検索
	std::map<std::string, Group>::iterator itGroup = dates_.find(groupName);

	//未登録チェック
	assert(itGroup != dates_.end());

	json root;

	root = json::object();

	//jsonオブジェクト登録
	root[groupName] = json::object();

	//グループの参照を取得
	Group& group = itGroup->second;

	//各項目について
	for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {
		//各項名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;

		//int32_tを保持してたら
		if (std::holds_alternative<int32_t>(item.value)) {
			//int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}
		//floatを保持してたら
		else if (std::holds_alternative<float>(item.value)) {
			//float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}
		//Vector3
		else if (std::holds_alternative<Vector3>(item.value)) {
			//float型のjson配列登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = nlohmann::json::array({ value.x, value.y, value.z });
		}
	}

	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directory(kDirectoryPath);
	}

	//書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//書き込み用ファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込みように開く
	ofs.open(filePath);

	//ファイルオープン失敗か
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	//ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
}

void GlobalVariables::LoadFiles() {
	//ディレクトリがなければスキップする
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		//ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

void GlobalVariables::LoadFile(const std::string& groupName) {
	//読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);

	//ファイルオープン失敗か
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();

	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	//未登録チェック
	assert(itGroup != root.end());

	//各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		//アイテム名を取得
		const std::string& itemName = itItem.key();

		//int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			//int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}
		//float型の値を保持していれば
		else if (itItem->is_number_float()) {
			//float型の値を登録
			float value = itItem->get<float>();
			SetValue(groupName, itemName, value);
		}
		else if (itItem->is_array() && itItem->size() == 3) {
			//float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
		}
	}
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	//グループの参照を取得
	Group& group = dates_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	//設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	//グループの参照を取得
	Group& group = dates_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	//設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) {
	//グループの参照を取得
	Group& group = dates_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	//設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalVariables::AddValue(const std::string& groupName, const std::string& key, int32_t value) {
	//グループを検索
	std::map<std::string, Group>::iterator itGroup = dates_.find(groupName);
	//未登録チェック
	assert(itGroup != dates_.end());

	//グループの参照を取得
	Group& group = itGroup->second;

	//キーを検索
	std::map<std::string, Item>::iterator itKey = group.items.find(key);

	if (itKey == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddValue(const std::string& groupName, const std::string& key, float value) {
	//グループを検索
	std::map<std::string, Group>::iterator itGroup = dates_.find(groupName);
	//未登録チェック
	assert(itGroup != dates_.end());

	//グループの参照を取得
	Group& group = itGroup->second;

	//キーを検索
	std::map<std::string, Item>::iterator itKey = group.items.find(key);

	if (itKey == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddValue(const std::string& groupName, const std::string& key, const Vector3& value) {
	//グループを検索
	std::map<std::string, Group>::iterator itGroup = dates_.find(groupName);
	//未登録チェック
	assert(itGroup != dates_.end());

	//グループの参照を取得
	Group& group = itGroup->second;

	//キーを検索
	std::map<std::string, Item>::iterator itKey = group.items.find(key);

	if (itKey == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {
	//グループを検索
	std::map<std::string, Group>::const_iterator itGroup = dates_.find(groupName);
	//未登録チェック
	assert(itGroup != dates_.end());

	//グループの参照を取得
	Group group = itGroup->second;

	//キーを検索
	std::map<std::string, Item>::const_iterator itKey = group.items.find(key);

	assert(itKey != group.items.end());

	return std::get<int32_t>(itKey->second.value);
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {
	//グループを検索
	std::map<std::string, Group>::const_iterator itGroup = dates_.find(groupName);
	//未登録チェック
	assert(itGroup != dates_.end());

	//グループの参照を取得
	Group group = itGroup->second;

	//キーを検索
	std::map<std::string, Item>::const_iterator itKey = group.items.find(key);

	assert(itKey != group.items.end());

	return std::get<float>(itKey->second.value);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) const {
	//グループを検索
	std::map<std::string, Group>::const_iterator itGroup = dates_.find(groupName);
	//未登録チェック
	assert(itGroup != dates_.end());

	//グループの参照を取得
	Group group = itGroup->second;

	//キーを検索
	std::map<std::string, Item>::const_iterator itKey = group.items.find(key);

	assert(itKey != group.items.end());

	return std::get<Vector3>(itKey->second.value);
}