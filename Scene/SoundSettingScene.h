#pragma once
#include "Secne.h"
#include <string>

class InputState;

class SoundSettingScene : public Scene
{
public:
	SoundSettingScene(SceneManager& manager,int soundH);
	~SoundSettingScene();

	void Update(const InputState& input, Mouse& mouse);
	void Draw();
private:

	void NormalUpdate(const InputState& input,  Mouse& mouse);
	void BGMVolumeChange(const InputState& input,  Mouse& mouse);
	void SEVolumeChange(const InputState& input,  Mouse& mouse);

	void (SoundSettingScene::* m_updateFunc)(const InputState& , Mouse& );

	static constexpr int pw_width = 450;
	static constexpr int pw_height = 350;
	static constexpr int pw_start_x = (640 - pw_width) / 2 + 50;
	static constexpr int pw_start_y = (480 - pw_height) / 2 + 50;

	static constexpr int kPosX = pw_start_x + 10;
	static constexpr int kPosY = pw_start_y + 30;
	
	static constexpr int kSize = 20;

	enum class SoundType
	{
		soundTypeBGM,
		soundTypeSE,

		soundTypeBack,

		soundTypeMax,
	};
	struct SoundVolume
	{
		int x, y;//座標
		int color;//色
		std::wstring name;//名前
	};

	SoundVolume m_soundChange[static_cast<int>(SoundType::soundTypeMax)] =
	{
		{kPosX		,kPosY + kSize * static_cast<int>(SoundType::soundTypeBGM )+ 1,0xaaffaa,L"BGM Volume = %d"},
		{kPosX + 10,kPosY + kSize * static_cast<int>(SoundType::soundTypeSE )+ 2,0xffffff,L"SE Volume = %d"},
		{kPosX + 10,kPosY + kSize * static_cast<int>(SoundType::soundTypeBack) + 3,0xffffff,L"戻る"},
	};

	int m_selectNum = 0;

	int m_waitInterval = 60;//値が変化するインターバル
	int m_puressTime = 0;//押しっぱなしの時間
	
	int m_soundH;//サウンドハンドル
};

