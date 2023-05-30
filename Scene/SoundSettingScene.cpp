#include "SoundSettingScene.h"
#include <Dxlib.h>
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/Sound.h"
#include "SceneManager.h"

SoundSettingScene::SoundSettingScene(SceneManager& manager,int soundH) :Scene(manager), m_soundH(soundH), m_updateFunc(&SoundSettingScene::NormalUpdate)
{
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::Update(const InputState& input, Mouse& mouse)
{
	(this->*m_updateFunc)(input,mouse);
}

void SoundSettingScene::Draw()
{
	//サウンドセッティングウィンドウ
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x884444, true);
	//サウンドセッティング中メッセージ
	DrawString(pw_start_x + 10, pw_start_y + 10, L"SoundSetting...", 0xffffaa);
	//BGMの設定
	auto& soundMgr = SoundManager::GetInstance();
	DrawFormatString(m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].x, m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].y, 
		m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].color,
		m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].name.c_str(), soundMgr.GetBGMVolume());
	//SEの設定
	DrawFormatString(m_soundChange[static_cast<int>(SoundType::soundTypeSE)].x, m_soundChange[static_cast<int>(SoundType::soundTypeSE)].y,
		m_soundChange[static_cast<int>(SoundType::soundTypeSE)].color,
		m_soundChange[static_cast<int>(SoundType::soundTypeSE)].name.c_str(), soundMgr.GetSEVolume());

	DrawFormatString(m_soundChange[static_cast<int>(SoundType::soundTypeBack)].x, m_soundChange[static_cast<int>(SoundType::soundTypeBack)].y,
		m_soundChange[static_cast<int>(SoundType::soundTypeBack)].color,
		m_soundChange[static_cast<int>(SoundType::soundTypeBack)].name.c_str());
	//サウンドセッティングウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}

void SoundSettingScene::NormalUpdate(const InputState& input,  Mouse& mouse)
{
	bool isPress = false;
	int num = static_cast<int>(SoundType::soundTypeMax);
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % num;
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (num - 1)) % num;
		isPress = true;
	}

	//マウスで選択
	if (mouse.MouseSelect(m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].x, m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].x + kSize*14,
		m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].y, m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].y + kSize))
	{
		if (m_selectNum != static_cast<int>(SoundType::soundTypeBGM))
		{
			m_selectNum = static_cast<int>(SoundType::soundTypeBGM);
		}
		isPress = true;
	}
	else if (mouse.MouseSelect(m_soundChange[static_cast<int>(SoundType::soundTypeSE)].x, m_soundChange[static_cast<int>(SoundType::soundTypeSE)].x + kSize*14,
		m_soundChange[static_cast<int>(SoundType::soundTypeSE)].y, m_soundChange[static_cast<int>(SoundType::soundTypeSE)].y + kSize))
	{
		if (m_selectNum != static_cast<int>(SoundType::soundTypeSE))
		{
			m_selectNum = static_cast<int>(SoundType::soundTypeSE);
		}
		isPress = true;
	}
	else if (mouse.MouseSelect(m_soundChange[static_cast<int>(SoundType::soundTypeBack)].x, m_soundChange[static_cast<int>(SoundType::soundTypeBack)].x + kSize*2,
		m_soundChange[static_cast<int>(SoundType::soundTypeBack)].y, m_soundChange[static_cast<int>(SoundType::soundTypeBack)].y + kSize))
	{
		if (m_selectNum != static_cast<int>(SoundType::soundTypeBack))
		{
			m_selectNum = static_cast<int>(SoundType::soundTypeBack);
		}
		isPress = true;
	}

	if (isPress)
	{
		for (int i = 0; i < num; i++)
		{
			if (i == m_selectNum)
			{
				m_soundChange[i].x = pw_start_x + 10;
				m_soundChange[i].color = 0xaaffaa;
			}
			else
			{
				m_soundChange[i].x = pw_start_x + 20;
				m_soundChange[i].color = 0xffffff;
			}
		}
	}
	auto& soundMgr = SoundManager::GetInstance();

	if (input.IsTriggered(InputType::slect))
	{
		switch (m_selectNum)
		{
		case static_cast<int>(SoundType::soundTypeBGM):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].color = 0xffa000;
			m_updateFunc = &SoundSettingScene::BGMVolumeChange;
			return;
		case static_cast<int>(SoundType::soundTypeSE):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_soundChange[static_cast<int>(SoundType::soundTypeSE)].color = 0xffa000;
			m_updateFunc = &SoundSettingScene::SEVolumeChange;
			return;
		case static_cast<int>(SoundType::soundTypeBack):
			SoundManager::GetInstance().Play(SoundId::Back);
			soundMgr.SaveSoundConfig();
			m_manager.PopScene();
			return;
		default:
			break;
		}
	}
	if (input.IsTriggered(InputType::prev))
	{
		soundMgr.SaveSoundConfig();
		SoundManager::GetInstance().Play(SoundId::Back);
		m_selectNum = static_cast<int>(SoundType::soundTypeBack);
		m_manager.PopScene();
		return;
	}
}

void SoundSettingScene::BGMVolumeChange(const InputState& input,  Mouse& mouse)
{
	auto& soundMgr = SoundManager::GetInstance();
	int soundVolume = 0;
	//BGM
	if (input.IsPressed(InputType::up))
	{
		if (input.IsTriggered(InputType::up))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetBGMVolume() + 1;
			if (soundVolume >= 255)
			{
				soundVolume = 255;
			}
			SoundManager::GetInstance().Play(SoundId::Cursor);
			soundMgr.SetBGMVolume(soundVolume, m_soundH);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}
	if (input.IsPressed(InputType::down))
	{
		if (input.IsTriggered(InputType::down))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetBGMVolume() - 1;
			if (soundVolume <= 0)
			{
				soundVolume = 0;
			}
			SoundManager::GetInstance().Play(SoundId::Cursor);
			soundMgr.SetBGMVolume(soundVolume, m_soundH);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}
	//マウスのホイールで音量を変える
	int Rot = 0;
	Rot += GetMouseWheelRotVol();
	if (Rot != 0)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
	}
	soundVolume = soundMgr.GetBGMVolume() + Rot;
	if (soundVolume >= 255)
	{
		soundVolume = 255;
	}
	else if (soundVolume <= 0)
	{
		soundVolume = 0;
	}
	soundMgr.SetBGMVolume(soundVolume, m_soundH);

	if (input.IsTriggered(InputType::slect) || input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_soundChange[static_cast<int>(SoundType::soundTypeBGM)].color = 0xaaffaa;
		m_updateFunc = &SoundSettingScene::NormalUpdate;
		return;
	}
}

void SoundSettingScene::SEVolumeChange(const InputState& input, Mouse& mouse)
{
	auto& soundMgr = SoundManager::GetInstance();
	int soundVolume = 0;

	//SE
	if (input.IsPressed(InputType::up))
	{
		if (input.IsTriggered(InputType::up))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetSEVolume() + 1;
			if (soundVolume >= 255)
			{
				soundVolume = 255;
			}
			SoundManager::GetInstance().Play(SoundId::Cursor);
			soundMgr.SetSEVolume(soundVolume);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}
	if (input.IsPressed(InputType::down))
	{
		if (input.IsTriggered(InputType::down))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetSEVolume() - 1;
			if (soundVolume <= 0)
			{
				soundVolume = 0;
			}
			SoundManager::GetInstance().Play(SoundId::Cursor);
			soundMgr.SetSEVolume(soundVolume);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}

	//マウスのホイールで音量を変える
	int Rot = 0;
	Rot += GetMouseWheelRotVol();
	if (Rot != 0)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
	}
	soundVolume = soundMgr.GetSEVolume() + Rot;
	if (soundVolume >= 255)
	{
		soundVolume = 255;
	}
	else if (soundVolume <= 0)
	{
		soundVolume = 0;
	}
	soundMgr.SetSEVolume(soundVolume);

	if (input.IsTriggered(InputType::slect) || input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_soundChange[static_cast<int>(SoundType::soundTypeSE)].color = 0xaaffaa;
		m_updateFunc = &SoundSettingScene::NormalUpdate;
		return;
	}
}

