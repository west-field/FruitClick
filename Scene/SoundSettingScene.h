#pragma once
#include "Secne.h"
#include "../Util/menuInfo.h"

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

	enum class SoundType
	{
		soundTypeBGM,
		soundTypeSE,

		soundTypeBack,

		soundTypeMax,
	};

	Element m_soundChange[static_cast<int>(SoundType::soundTypeMax)];

	int m_selectNum = 0;

	int m_waitInterval = 60;//�l���ω�����C���^�[�o��
	int m_puressTime = 0;//�������ςȂ��̎���
	
	int m_soundH;//�T�E���h�n���h��

	int m_bg;
};

