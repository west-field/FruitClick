#include "ConfirmationScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/Sound.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"

namespace
{
	static constexpr int pw_width = 350;
	static constexpr int pw_height = 250;
	static constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	static constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	static constexpr int kPosY = (pw_start_y + pw_height) - (pw_height / 2);

	static constexpr int kFontSize = 20;
}

ConfirmationScene::ConfirmationScene(SceneManager& manager, const wchar_t* conf, bool isEnd,int soundH) : 
	Scene(manager), m_isEnd(isEnd), m_soundH(soundH)
{
	m_conf += conf;

	m_pauseMenu[static_cast<int>(Item::yes)].x = pw_start_x + 20; 
	m_pauseMenu[static_cast<int>(Item::yes)].y = kPosY;
	m_pauseMenu[static_cast<int>(Item::yes)].name = L"�͂�";
		
	m_pauseMenu[static_cast<int>(Item::no)].x = (pw_start_x + pw_width) - (pw_width / 2);
	m_pauseMenu[static_cast<int>(Item::no)].y = kPosY ;
	m_pauseMenu[static_cast<int>(Item::no)].name = L"������";

	m_selectNum = static_cast<int>(Item::no);

	for (int i = 0; i < static_cast<int>(Item::Max); i++)
	{
		if (i == m_selectNum)
		{
			m_pauseMenu[i].size = kFontSize * 2;
			m_pauseMenu[i].color = 0x00ff00;
		}
		else
		{
			m_pauseMenu[i].size = kFontSize;
			m_pauseMenu[i].color = 0x000000;
		}
	}
}

ConfirmationScene::~ConfirmationScene()
{

}

void ConfirmationScene::Update(const InputState& input, Mouse& mouse)
{
	bool isSelect = false;
	int pauseMax = static_cast<int>(Item::Max);
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % pauseMax;
		isSelect = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (pauseMax - 1)) % pauseMax;
		isSelect = true;
	}

	//�}�E�X�őI��
	if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::yes)].x, m_pauseMenu[static_cast<int>(Item::yes)].x + kFontSize*6,
		m_pauseMenu[static_cast<int>(Item::yes)].y, m_pauseMenu[static_cast<int>(Item::yes)].y + kFontSize))
	{
		m_selectNum = static_cast<int>(Item::yes);
		isSelect = true;
	}
	else if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::no)].x, m_pauseMenu[static_cast<int>(Item::no)].x + kFontSize*2,
		m_pauseMenu[static_cast<int>(Item::no)].y, m_pauseMenu[static_cast<int>(Item::no)].y + kFontSize))
	{
		m_selectNum = static_cast<int>(Item::no);
		isSelect = true;
	}

	if (isSelect)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
		for (int i = 0; i < pauseMax; i++)
		{
			if (i == m_selectNum)
			{
				m_pauseMenu[i].size = kFontSize * 2;
				m_pauseMenu[i].color = 0x00ff00;
			}
			else
			{
				m_pauseMenu[i].size = kFontSize;
				m_pauseMenu[i].color = 0x000000;
			}
		}
	}

	if (input.IsTriggered(InputType::slect))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		switch (m_selectNum)
		{
		case static_cast<int>(Item::yes):
			if (m_isEnd)
			{
				m_manager.SetIsEnd();
			}
			else
			{
				m_manager.ChangeScene(new TitleScene(m_manager));
			}
			StopSoundMem(m_soundH);
			return;
		case static_cast<int>(Item::no):
			m_manager.PopScene();
			return;
		default:
			break;
		}
	}
	if (input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_selectNum = static_cast<int>(Item::no);
		m_manager.PopScene();
		return;
	}
}

void ConfirmationScene::Draw()
{
	//�E�B���h�E�Z���t�@��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xaaaaaa, true);
	//���b�Z�[�W
	SetFontSize(kFontSize);
	DrawString(pw_start_x + 30, pw_start_y + 10, m_conf.c_str(), 0xf0f088);
	for (auto& menu : m_pauseMenu)
	{
		SetFontSize(menu.size);
		DrawString(menu.x, menu.y, menu.name, menu.color);
	}
		SetFontSize(0);
	//�E�B���h�E�g��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, false);
}