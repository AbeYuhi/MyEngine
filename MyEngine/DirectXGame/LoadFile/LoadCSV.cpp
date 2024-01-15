#include "LoadCSV.h"

std::stringstream LoadCSV::Load(std::string fileName) {

    std::string filePath = "Resources/ScriptFilles/" + fileName;

    //ファイルを開く
    std::ifstream file;
    file.open(filePath);
    assert(file.is_open());
    //ファイルの内容を文字列ストリームにコピー
    std::stringstream stringStream;
    stringStream << file.rdbuf();

    //ファイルを閉じる
    file.close();

    return stringStream;
}