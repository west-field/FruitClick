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
#include "ExplanationScene.h"

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

	m_selectNum = -1;

	for (int i = 0; i < static_cast<int>(Item::Max); i++)
	{
		m_pauseMenu[i].fontSize = kFontSize;
		m_pauseMenu[i].color = 0x000000;
		m_pauseMenu[i].nameNum = static_cast<int>(wcslen(m_pauseMenu[i].name));
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
	m_selectNum = -1;

	//マウスで選択
	int i = 0;
	for (auto& menu : m_pauseMenu)
	{
		if (mouse.MouseSelect(menu.x, menu.x + menu.fontSize * menu.nameNum, menu.y, menu.y + menu.fontSize))
		{
			if (m_selectNum != i)
			{
				m_selectNum = i;
			}
			isSelect = true;
			break;
		}
		i++;
	}
	
	for (int i = 0; i < pauseMax; i++)
	{
		if (i == m_selectNum)
		{
			m_pauseMenu[i].fontSize = kFontSize * 2;
			m_pauseMenu[i].color = 0x00ff00;
		}
		else
		{
			m_pauseMenu[i].fontSize = kFontSize;
			m_pauseMenu[i].color = 0x000000;
		}
	}

	if (isSelect && input.IsTriggered(InputType::slect))
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
			else if(m_type == SelectType::SceneMode)
			{
				FullSceneChange();
				m_manager.PopScene();
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
		SetFontSize(menu.fontSize);
		DrawString(menu.x, menu.y, menu.name, menu.color);
#ifdef _DEBUG
		int size = menu.nameNum * menu.fontSize;
		DrawBox(menu.x, menu.y, menu.x + size, menu.y + menu.fontSize, menu.color, false);
#endif
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
