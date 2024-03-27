#include "Sound.h"
#include <DxLib.h>
#include <cassert>
#include <string>

namespace
{
	constexpr char kSoundConfigFilePath[] = "Data/sound.conf";
}

SoundManager::SoundManager()
{
	//サウンド情報のロード
	LoadSoundConfig();

	//サウンドハンドルを取得する
	LoadSoundFile(SoundId::Cursor,L"Cursor.mp3");
	LoadSoundFile(SoundId::Determinant,L"Determinant.mp3");
	LoadSoundFile(SoundId::Back,L"Back.mp3");
	
	LoadSoundFile(SoundId::Count,L"Count.mp3");
	LoadSoundFile(SoundId::Start,L"Start.mp3");
	LoadSoundFile(SoundId::Point,L"Point.mp3");

	LoadSoundFile(SoundId::PlayerMove,L"Move.mp3");
	LoadSoundFile(SoundId::Hit,L"Hit.mp3");
	
	LoadSoundFile(SoundId::FruitCreate,L"FruitCreate.mp3");
	LoadSoundFile(SoundId::FruitClick,L"FruitClick.mp3");
	LoadSoundFile(SoundId::FruitDelete,L"FruitDelete.mp3");
}
SoundManager::~SoundManager()
{
}

void SoundManager::LoadSoundFile(SoundId id, const wchar_t* fileName)
{
	//サウンドハンドルを取得する
	//パスとデータ名を一つにする
	std::wstring path = L"Data/Sound/SE/";
	path += fileName;
	int handle = LoadSoundMem(path.c_str());

	assert(handle >= 0);
	m_nameAndHandleTable[id] = handle;
}

void SoundManager::LoadSoundConfig()
{
	//サウンド情報のロード
	SoundConfigInfo conf = {};

	//ファイルから情報を取得する
	FILE* fp = nullptr;
	fopen_s(&fp, kSoundConfigFilePath, "rb");

	if (fp)
	{
		fread(&conf, sizeof(conf), 1, fp);
		fclose(fp);
		m_volumeBGM = conf.volumeBGM;
		m_volumeSE = conf.volumeSE;
	}
}

void SoundManager::SaveSoundConfig()
{
	//サウンド情報のセーブ
	SoundConfigInfo conf = {};
	conf.volumeSE = m_volumeSE;
	conf.volumeBGM = m_volumeBGM;

	FILE* fp = nullptr;
	fopen_s(&fp, kSoundConfigFilePath, "wb");
	if (fp)
	{
		fwrite(&conf, sizeof(conf), 1, fp);
		fclose(fp);
	}
}

void SoundManager::Play(SoundId id, int volume)
{
	//指定のSEを鳴らす
	PlaySoundMem(m_nameAndHandleTable[id], DX_PLAYTYPE_BACK);
}

void SoundManager::PlayBGM(int soundH) const
{
	//BGMを鳴らす
	PlaySoundMem(soundH, DX_PLAYTYPE_LOOP);
	//音量を設定する
	ChangeVolumeSoundMem(m_volumeBGM,soundH);
}

void SoundManager::SetSEVolume(int volume)
{
	//全ての音の音量を変更する
	for (auto& record : m_nameAndHandleTable)
	{
		ChangeVolumeSoundMem(volume, record.second);
	}
	m_volumeSE = volume;
}

int SoundManager::GetSEVolume() const
{
	return m_volumeSE;
}

void SoundManager::SetBGMVolume(int volume,int soundH)
{
	ChangeVolumeSoundMem(volume,soundH);
	m_volumeBGM = volume;
}

int SoundManager::GetBGMVolume() const
{
	return m_volumeBGM;
}

void SoundManager::StopBgm(SoundId id)
{
	StopSoundMem(m_nameAndHandleTable[id]);
}
