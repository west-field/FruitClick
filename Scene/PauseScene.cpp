#include "PauseScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Mouse.h"
#include "../Util/Sound.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "SoundSettingScene.h"
#include "ConfirmationScene.h"

namespace
{
	static constexpr int kWindowWidth = 400;
	static constexpr int kWindowHeight = 300;
	static constexpr int kWindowStartX = (Game::kScreenWidth - kWindowWidth) / 2;
	static constexpr int kWindowStartY = (Game::kScreenHeight - kWindowHeight) / 2;

	static constexpr int kPosX = kWindowStartX + 10;
	static constexpr int kPosY = kWindowStartY + 10;

	static constexpr int kFontSize = 20;
}

PauseScene::PauseScene(SceneManager& manager,int soundH) : Scene(manager), m_soundH(soundH)
{
	m_selectNum = -1;

	m_pauseMenu[static_cast<int>(Item::pauseSound)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseSound)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseSound) + 1) + 10;
	m_pauseMenu[static_cast<int>(Item::pauseSound)].name = L"音量せってい";

	m_pauseMenu[static_cast<int>(Item::pauseFullscreen)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseFullscreen)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseFullscreen) + 1) + 20;
	m_pauseMenu[static_cast<int>(Item::pauseFullscreen)].name = L"スクリーンモード変更";

	m_pauseMenu[static_cast<int>(Item::pauseBack)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseBack) + 1) + 30;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].name = L"閉じる";

	m_pauseMenu[static_cast<int>(Item::pauseTitle)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseTitle)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseTitle) + 1) + 40;
	m_pauseMenu[static_cast<int>(Item::pauseTitle)].name = L"タイトル戻る";

	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseGameEnd) + 1) + 50;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].name = L"ゲーム終了";

	int num = static_cast<int>(Item::pauseMax);
	for (int i = 0; i < num; i++)
	{
		m_pauseMenu[i].fontSize = kFontSize;
		m_pauseMenu[i].color = 0x000080;
		m_pauseMenu[i].nameNum = static_cast<int>(wcslen(m_pauseMenu[i].name));
	}
	m_bg = my::MyLoadGraph(L"Data/panel_Example3.png");
}

PauseScene::~PauseScene()
{
	DeleteGraph(m_bg);
}

void PauseScene::Update(Mouse& mouse)
{
	bool isSelect = false;
	int num = static_cast<int>(Item::pauseMax);
	m_selectNum = -1;

	//選択
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

	for (int i = 0; i < num; i++)
	{
		if (i == m_selectNum)
		{
			m_pauseMenu[i].fontSize = kFontSize + 15;
			m_pauseMenu[i].color = 0xaaffaa;
		}
		else
		{
			m_pauseMenu[i].fontSize = kFontSize;
			m_pauseMenu[i].color = 0x000080;
		}
	}

	if (isSelect && mouse.IsTrigger(Mouse::InputType::left))
	{
		switch (m_selectNum)
		{
		case static_cast<int>(Item::pauseSound):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new SoundSettingScene(m_manager, m_soundH));
			return;
		case static_cast<int>(Item::pauseFullscreen):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"スクリーンモード変更しますか", SelectType::SceneMode, m_soundH));
			return;
		case static_cast<int>(Item::pauseBack):
			SoundManager::GetInstance().Play(SoundId::Back);
			m_manager.PopScene();
			return;
		case static_cast<int>(Item::pauseTitle):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"タイトルに戻りますか", SelectType::BackTitle, m_soundH));
			return;
		case static_cast<int>(Item::pauseGameEnd):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"ゲームを終了しますか", SelectType::End, m_soundH));
			return;
		default:
			break;
		}
	}

	if (mouse.IsTrigger(Mouse::InputType::right))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_selectNum = static_cast<int>(Item::pauseBack);
		m_manager.PopScene();
		return;
	}
}

void PauseScene::Draw()
{
	//ウィンドウ背景
	WindowDraw();
	//ポーズ中メッセージ
	SetFontSize(kFontSize * 2);

	DrawString(kWindowStartX + 10+ (kFontSize * 2) * 3, kWindowStartY + 10, L"ポーズ", 0x000000);
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

void PauseScene::WindowDraw()
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
