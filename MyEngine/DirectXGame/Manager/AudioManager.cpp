#include "AudioManager.h"
#pragma comment(lib, "xaudio2.lib")

AudioManager* AudioManager::GetInstance() {
	static AudioManager instance;
	return &instance;
}

void AudioManager::Initialize() {
	HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));

	textureNum = 0;
}

void AudioManager::Finalize() {
	xAudio2_.Reset();

	for (int index = 0; index < textureNum; index++) {
		SoundUnload(index);
	}
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
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
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
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	//returnするための音声データ
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	soundDatas_[index] = soundData;
	return index;
}

void AudioManager::SoundPlayWave(const uint32_t index, const float soundVolume, bool isLoop) {
	HRESULT hr;

	hr = xAudio2_->CreateSourceVoice(&soundDatas_[index].pSourceVoice, &soundDatas_[index].wfex);
	assert(SUCCEEDED(hr));

	//再生する波形データの設定
	soundDatas_[index].buf = {};
	soundDatas_[index].buf.pAudioData = soundDatas_[index].pBuffer;
	soundDatas_[index].buf.AudioBytes = soundDatas_[index].bufferSize;
	soundDatas_[index].buf.Flags = XAUDIO2_END_OF_STREAM;

	if (isLoop) {
		soundDatas_[index].buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//波形データの再生
	hr = soundDatas_[index].pSourceVoice->SubmitSourceBuffer(&soundDatas_[index].buf);
	hr = soundDatas_[index].pSourceVoice->Start();
	soundDatas_[index].pSourceVoice->SetVolume(soundVolume);
}

void AudioManager::StopLoopWave(const uint32_t index) {
	if (soundDatas_[index].pSourceVoice) {
		soundDatas_[index].pSourceVoice->Stop();
		soundDatas_[index].pSourceVoice->FlushSourceBuffers();
	}
}

bool AudioManager::IsSoundPlaying(const uint32_t index) {
	if (soundDatas_[index].pSourceVoice) {
		XAUDIO2_VOICE_STATE voiceState;
		soundDatas_[index].pSourceVoice->GetState(&voiceState);
		return voiceState.BuffersQueued > 0;
	}
	return false;
}


void AudioManager::SoundUnload(const uint32_t index) {
	delete[] soundDatas_[index].pBuffer;

	soundDatas_[index].pBuffer = 0;
	soundDatas_[index].bufferSize = 0;
	soundDatas_[index].wfex = {};
}