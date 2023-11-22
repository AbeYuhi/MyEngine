#include "AudioManager.h"
#pragma comment(lib, "xaudio2.lib")

AudioManager* AudioManager::GetInstance() {
	static AudioManager instance;
	return &instance;
}

void AudioManager::Initialize() {
	WinApp* winApp = WinApp::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	
	HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));

	textureNum = 0;
}

void AudioManager::Finalize() {
	xAudio2_.Reset();
}

uint32_t AudioManager::SoundLoadWave(const std::string filename) {
	textureNum++;
	uint32_t index = textureNum - 1;

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/Sounds/" + filename, std::ios_base::binary);
	assert(file.is_open());

	//データ読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがriffかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt", 4) != 0) {
		assert(0);
	}
	//本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}
	//データチャンクの読み込み
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}
	//Dataチャンクのデータ部の読み込み
	std::unique_ptr<char> pBuffer = std::make_unique<char>(data.size);
	file.read(pBuffer.get(), data.size);

	//Waveファイルを閉じる
	file.close();

	//returnするための音声データ
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer.get());
	soundData.bufferSize = data.size;

	soundDates_.push_back(soundData);
	soundHandles_[index] = &soundDates_[index];
	return index;
}

void AudioManager::SoundPlayWave(const uint32_t index) {
	HRESULT hr;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundHandles_[index]->wfex);
	assert(SUCCEEDED(hr));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundHandles_[index]->pBuffer;
	buf.AudioBytes = soundHandles_[index]->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	hr = pSourceVoice->Start();
}