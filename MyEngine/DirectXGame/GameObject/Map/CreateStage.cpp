#include "CreateStage.h"

std::list<std::unique_ptr<StageBlock>> CreateStage(std::string stageFileName, Vector3 size) {
	//返り値
	std::list<std::unique_ptr<StageBlock>> stageBlockList;


	//ステージの読み込み
	std::stringstream stageInfo = LoadCSV::Load(stageFileName);

	//1行分の文字列を入れる変数
	std::string line;
	std::list<std::string> lines;

	//コマンド実行ループ
	while (getline(stageInfo, line)){

		//コメントアウト部分を飛ばす処理
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}

		lines.push_back(line);
	}
	std::reverse(lines.begin(), lines.end());

	int y = 0;
	for (std::string a : lines) {
		std::istringstream line_stream(a);
		std::string word;

		//,区切りで行の先頭文字列を取得
		int x = 0;
		while (getline(line_stream, word, ',')) {
			int blockID = (int)std::atoi(word.c_str());
			if (blockID != 0) {
				std::unique_ptr<StageBlock> stageBlock = std::make_unique<StageBlock>();
				stageBlock->stageModel = Model::Create("cube", "cube.obj");
				stageBlock->stageModelInfo.Initialize();
				stageBlock->stageModelInfo.worldTransform_.data_.translate_ = { (float)x * size.x, (float)y * size.y, 0.0f };
				stageBlock->stageModelInfo.worldTransform_.data_.scale_ = size;
				stageBlock->blockInfo = blockID;
				stageBlock->isBreak = false;
				stageBlockList.push_back(std::move(stageBlock));
			}
			x++;
		}
		y++;
	}

	return stageBlockList;
}