#pragma once
#include "Secne.h"
#include "../Util/menuInfo.h"

/// <summary>
/// サウンド設定
/// </summary>
class SoundSettingScene : public Scene
{
public:
	SoundSettingScene(SceneManager& manager,int soundH);
	~SoundSettingScene();

	void Update(Mouse& mouse);
	void Draw();
private:

	void NormalUpdate( Mouse& mouse);
	void BGMVolumeChange( Mouse& mouse);
	void SEVolumeChange( Mouse& mouse);

	void WindowDraw();

	void (SoundSettingScene::* m_updateFunc)( Mouse& );

	/// <summary>
	/// サウンド設定の項目
	/// </summary>
	enum class SoundItem
	{
		soundTypeBGM,
		soundTypeSE,

		soundTypeBack,

		soundTypeMax,
	};

	Element m_soundChange[static_cast<int>(SoundItem::soundTypeMax)];

	int m_selectNum = 0;

	int m_waitInterval = 60;//値が変化するインターバル
	int m_puressTime = 0;//押しっぱなしの時間
	
	int m_soundH;//サウンドハンドル

	int m_bg;
};

