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

ConfirmationScene::ConfirmationScene(SceneManager& manager, const wchar_t* conf, SelectType type,int soundH) :
	Scene(manager), m_type(type), m_soundH(soundH)
{
	m_conf += conf;

	m_pauseMenu[static_cast<int>(Item::yes)].x = (pw_start_x + pw_width) - (pw_width / 2);
	m_pauseMenu[static_cast<int>(Item::yes)].y = kPosY;
	m_pauseMenu[static_cast<int>(Item::yes)].name = L"はい";
		
	m_pauseMenu[static_cast<int>(Item::no)].x = pw_start_x + 20; 
	m_pauseMenu[static_cast<int>(Item::no)].y = kPosY ;
	m_pauseMenu[static_cast<int>(Item::no)].name = L"いいえ";

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
	m_isWindouwMode = m_manager.GetIsWindouMode();
}

ConfirmationScene::~ConfirmationScene()
{

}

void ConfirmationScene::Update(const InputState& input, Mouse& mouse)
{
	bool isSelect = false;
	int pauseMax = static_cast<int>(Item::Max);
	if (input.IsTriggered(InputType::down) || input.IsTriggered(InputType::left))
	{
		m_selectNum = (m_selectNum + 1) % pauseMax;
		SoundManager::GetInstance().Play(SoundId::Cursor);
		isSelect = true;
	}
	else if (input.IsTriggered(InputType::up) || input.IsTriggered(InputType::right))
	{
		m_selectNum = (m_selectNum + (pauseMax - 1)) % pauseMax;
		SoundManager::GetInstance().Play(SoundId::Cursor);
		isSelect = true;
	}

	//マウスで選択
	if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::yes)].x, m_pauseMenu[static_cast<int>(Item::yes)].x + kFontSize*6,
		m_pauseMenu[static_cast<int>(Item::yes)].y, m_pauseMenu[static_cast<int>(Item::yes)].y + kFontSize))
	{
		if (m_selectNum != static_cast<int>(Item::yes))
		{
			m_selectNum = static_cast<int>(Item::yes);
			SoundManager::GetInstance().Play(SoundId::Cursor);
		}
		isSelect = true;
	}
	else if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::no)].x, m_pauseMenu[static_cast<int>(Item::no)].x + kFontSize*2,
		m_pauseMenu[static_cast<int>(Item::no)].y, m_pauseMenu[static_cast<int>(Item::no)].y + kFontSize))
	{
		if (m_selectNum != static_cast<int>(Item::no))
		{
			m_selectNum = static_cast<int>(Item::no);
			SoundManager::GetInstance().Play(SoundId::Cursor);
		}
		isSelect = true;
	}

	if (isSelect)
	{
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
		switch (m_selectNum)
		{
		case static_cast<int>(Item::yes):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			if (m_type == SelectType::End)
			{
				m_manager.SetIsEnd();
			}
			else if(m_type == SelectType::BackTitle)
			{
				m_manager.ChangeScene(new TitleScene(m_manager));
			}
			else if(m_type == SelectType::Scene)
			{
				FullSceneChange();
				m_manager.PopScene();
				return;
			}

			StopSoundMem(m_soundH);
			return;
		case static_cast<int>(Item::no):
			SoundManager::GetInstance().Play(SoundId::Back);
			m_manager.PopScene();
			return;
		default:
			break;
		}
	}
	if (input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_selectNum = static_cast<int>(Item::no);
		m_manager.PopScene();
		return;
	}
}

void ConfirmationScene::Draw()
{
	//ウィンドウセロファン
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xaaaaaa, true);
	//メッセージ
	SetFontSize(kFontSize);
	DrawString(pw_start_x + 30, pw_start_y + 10, m_conf.c_str(), 0xf0f088);
	for (auto& menu : m_pauseMenu)
	{
		SetFontSize(menu.size);
		DrawString(menu.x, menu.y, menu.name, menu.color);
	}
		SetFontSize(0);
	//ウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, false);
}

void ConfirmationScene::FullSceneChange()
{
	
	m_isWindouwMode = !m_isWindouwMode;
	ChangeWindowMode(m_isWindouwMode);
	m_manager.SetIsWindouMode(m_isWindouwMode);
	SetDrawScreen(DX_SCREEN_BACK);//描画先を再定義
}
