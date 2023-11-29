#pragma once
#include "DirectXGame/Base/WinApp/WinApp.h"
#include "DirectXGame/Base/DirectXCommon/DirectXCommon.h"
#include <xaudio2.h>
#include <fstream>
#include <map>

struct ChunkHeader {
	char id[4];
	int32_t size;
};

struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData {
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;
	//再生時のボイス
	IXAudio2SourceVoice* pSourceVoice;
	//バッファ
	XAUDIO2_BUFFER buf;
};

class AudioManager
{
public:
	static AudioManager* GetInstance();

	void Initialize();

	void Finalize();

	uint32_t SoundLoadWave(const std::string filename);

	void SoundPlayWave(const uint32_t index, const float soundVolume = 1.0f, bool isLoop = false);

	void StopLoopWave(const uint32_t index);

	bool IsSoundPlaying(const uint32_t index);

	void SoundUnload(const uint32_t index);

	//ゲッターセッター
	inline void SetVolume(const uint32_t index, const float soundVolume) {
		if (soundDatas_[index].pSourceVoice) {
			soundDatas_[index].pSourceVoice->SetVolume(soundVolume);
		}
	}

private: //メンバ関数

private:
	AudioManager() = default;
	~AudioManager() = default;

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
	std::map<uint32_t, SoundData> soundDatas_;
	int textureNum;
};