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
};

class AudioManager
{
public:
	static AudioManager* GetInstance();

	void Initialize();

	void Finalize();

	uint32_t SoundLoadWave(const std::string filename);

	void SoundPlayWave(const uint32_t index);

private: //メンバ関数

private:
	AudioManager() = default;
	~AudioManager() = default;

	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;
	std::vector<SoundData> soundDates_;
	std::map<uint32_t, SoundData*> soundHandles_;
	int textureNum;
};