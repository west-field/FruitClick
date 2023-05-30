#include "PauseScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/Sound.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"
#include "ConfirmationScene.h"

namespace
{
	static constexpr int pw_width = 400;
	static constexpr int pw_height = 300;
	static constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	static constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	static constexpr int kPosX = pw_start_x + 10;
	static constexpr int kPosY = pw_start_y + 10;

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
	m_pauseMenu[static_cast<int>(Item::pauseFullscreen)].name = L"フルスクリーン変更";

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
		m_pauseMenu[i].color = 0xffffff;
		m_pauseMenu[i].nameNum = static_cast<int>(wcslen(m_pauseMenu[i].name));
	}
}

PauseScene::~PauseScene()
{

}

void PauseScene::Update(const InputState& input, Mouse& mouse)
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
			m_pauseMenu[i].fontSize = kFontSize * 2;
			m_pauseMenu[i].color = 0xaaffaa;
		}
		else
		{
			m_pauseMenu[i].fontSize = kFontSize;
			m_pauseMenu[i].color = 0xffffff;
		}
	}

	if (isSelect && input.IsTriggered(InputType::slect))
	{
		switch (m_selectNum)
		{
		case static_cast<int>(Item::pauseSound):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new SoundSettingScene(m_manager, m_soundH));
			return;
		case static_cast<int>(Item::pauseFullscreen):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"フルスクリーン変更しますか", SelectType::SceneMode, m_soundH));
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

	if (input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_selectNum = static_cast<int>(Item::pauseBack);
		m_manager.PopScene();
		return;
	}
}

void PauseScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//ポーズウィンドウセロファン
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//ポーズ中メッセージ
	SetFontSize(kFontSize * 2);
	DrawString(pw_start_x + 10, pw_start_y + 10, L"ポーズ", 0xffff88);
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
	//ポーズウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}
