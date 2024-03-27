#include "ConfirmationScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Mouse.h"
#include "../Util/Sound.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "SoundSettingScene.h"
#include "ExplanationScene.h"

namespace
{
	static constexpr int kWindowWidth = 350;
	static constexpr int kWindowHeight = 250;
	static constexpr int kWindowStartX = (Game::kScreenWidth - kWindowWidth) / 2;
	static constexpr int kWindowStartY = (Game::kScreenHeight - kWindowHeight) / 2;

	static constexpr int kPosY = (kWindowStartY + kWindowHeight) - (kWindowHeight / 2);

	static constexpr int kFontSize = 20;
}

ConfirmationScene::ConfirmationScene(SceneManager& manager, const wchar_t* conf, SelectType type,int soundH) :
	Scene(manager), m_type(type), m_soundH(soundH),m_conf(conf)
{
	m_stringNum = static_cast<int>(wcslen(m_conf));

	m_pauseMenu[static_cast<int>(Choice::yes)].x = (kWindowStartX + kWindowWidth) - (kWindowWidth / 2);
	m_pauseMenu[static_cast<int>(Choice::yes)].y = kPosY;
	m_pauseMenu[static_cast<int>(Choice::yes)].name = L"はい";
		
	m_pauseMenu[static_cast<int>(Choice::no)].x = kWindowStartX + 20; 
	m_pauseMenu[static_cast<int>(Choice::no)].y = kPosY ;
	m_pauseMenu[static_cast<int>(Choice::no)].name = L"いいえ";

	m_selectNum = -1;

	for (int i = 0; i < static_cast<int>(Choice::Max); i++)
	{
		m_pauseMenu[i].fontSize = kFontSize;
		m_pauseMenu[i].color = 0x000000;
		m_pauseMenu[i].nameNum = static_cast<int>(wcslen(m_pauseMenu[i].name));
	}
	m_isWindowMode = m_manager.GetIsWindouMode();
	m_bg = my::MyLoadGraph(L"Data/panel_Example2.png");
}

ConfirmationScene::~ConfirmationScene()
{
	DeleteGraph(m_bg);
}

void ConfirmationScene::Update( Mouse& mouse)
{
	bool isSelect = false;
	int pauseMax = static_cast<int>(Choice::Max);
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

	if (isSelect && mouse.IsTrigger(Mouse::InputType::left))
	{
		switch (m_selectNum)
		{
		case static_cast<int>(Choice::yes):
			StopSoundMem(m_soundH);
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
				WindowModeChange();
				m_manager.PopScene();
			}
			return;
		case static_cast<int>(Choice::no):
			SoundManager::GetInstance().Play(SoundId::Back);
			m_manager.PopScene();
			return;
		default:
			break;
		}
	}
	if (mouse.IsTrigger(Mouse::InputType::right))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_selectNum = static_cast<int>(Choice::no);
		m_manager.PopScene();
		return;
	}
}

void ConfirmationScene::Draw()
{
	//ウィンドウ背景
	WindowDraw();
	//メッセージ
	SetFontSize(kFontSize);
	//DrawString(pw_start_x + 30, pw_start_y + 10, m_conf, 0xf0f088);
	int startString = (kWindowWidth - (m_stringNum * kFontSize)) / 2;
	DrawString(kWindowStartX + startString, kWindowStartY + kFontSize*2, m_conf,0xff0000 );//0xf0f088
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
}

void ConfirmationScene::WindowDraw()
{
	//角表示
	int x = kWindowStartX - 50 / 2;
	int y = kWindowStartY - 50 / 2;
	DrawRectGraph(x, y,
		0, 0, 50, 50, m_bg, true);//左上　50 y3,x9
	DrawRectGraph(x + kWindowWidth, y,
		50 * 8, 0, 50, 50, m_bg, true);//右上
	DrawRectGraph(x, y + kWindowHeight,
		0, 50 * 2, 50, 50, m_bg, true);//左下　50 y3,x9
	DrawRectGraph(x + kWindowWidth, y + kWindowHeight,
		50 * 8, 50 * 2, 50, 50, m_bg, true);//右下

	//画像の左上、右下、グラフィックの左上からXサイズ、Yサイズ、表示する画像、透明
	DrawRectExtendGraph(x + 50, y, x + kWindowWidth, y + 50,
		50 * 2, 0, 50, 50, m_bg, true);//上
	DrawRectExtendGraph(x, y + 50, x + 50, y + kWindowHeight,
		0, 50 * 1, 50, 50, m_bg, true);//左
	DrawRectExtendGraph(x + kWindowWidth, y + 50, x + kWindowWidth + 50, y + kWindowHeight,
		50 * 8, 50, 50, 50, m_bg, true);// 右
	DrawRectExtendGraph(x + 50, y + kWindowHeight, x + kWindowWidth, y + kWindowHeight + 50,
		50 * 2, 50 * 2, 50, 50, m_bg, true);	// 下
	DrawRectExtendGraph(x + 50, y + 50, x + kWindowWidth, y + kWindowHeight,
		50 * 3, 50 * 1, 50, 50, m_bg, true);	// ウインドウ内部
}

void ConfirmationScene::WindowModeChange()
{
	m_isWindowMode = !m_isWindowMode;
	ChangeWindowMode(m_isWindowMode);
	m_manager.SetIsWindouMode(m_isWindowMode);
	SetDrawScreen(DX_SCREEN_BACK);//描画先を再定義

	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume(), m_soundH);
	PlaySoundMem(m_soundH, DX_PLAYTYPE_LOOP, true);
}
