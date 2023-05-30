#include "SettingScene.h"
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
	 constexpr int pw_width = 400;
	 constexpr int pw_height = 300;
	 constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	 constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	 constexpr int kPosX = pw_start_x + 10;
	 constexpr int kPosY = pw_start_y + 10;

	 constexpr int kFontSize = 20;
}

SettingScene::SettingScene(SceneManager& manager,int soundH) : Scene(manager), m_soundH(soundH)
{
	m_selectNum = static_cast<int>(Item::pauseBack);

	m_pauseMenu[static_cast<int>(Item::pauseSound)].x = kPosX +10;
	m_pauseMenu[static_cast<int>(Item::pauseSound)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseSound) + 1) + 10;
	m_pauseMenu[static_cast<int>(Item::pauseSound)].name = L"音量せってい";
	
	m_pauseMenu[static_cast<int>(Item::pauseFullScene)].x = kPosX +10;
	m_pauseMenu[static_cast<int>(Item::pauseFullScene)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseFullScene) + 1) + 20;
	m_pauseMenu[static_cast<int>(Item::pauseFullScene)].name = L"フルスクリーン変更";

	m_pauseMenu[static_cast<int>(Item::pauseBack)].x = kPosX +10;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseBack) + 1) + 30;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].name = L"閉じる";

	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].x = kPosX+10;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseGameEnd) + 1) + 40;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].name = L"ゲーム終了";

	int pauseMax = static_cast<int>(Item::pauseMax);
	for (int i = 0; i < pauseMax; i++)
	{
		if (i == m_selectNum)
		{
			m_pauseMenu[i].size = kFontSize * 2;
			m_pauseMenu[i].color = 0xaaffaa;
		}
		else
		{
			m_pauseMenu[i].size = kFontSize;
			m_pauseMenu[i].color = 0xffffff;
		}
	}
}

SettingScene::~SettingScene()
{

}

void SettingScene::Update(const InputState& input, Mouse& mouse)
{
	bool isSelect = false;
	int pauseMax = static_cast<int>(Item::pauseMax);
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % pauseMax;
		SoundManager::GetInstance().Play(SoundId::Cursor);
		isSelect = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (pauseMax - 1)) % pauseMax;
		SoundManager::GetInstance().Play(SoundId::Cursor);
		isSelect = true;
	}

	//マウスで選択
	if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::pauseSound)].x, m_pauseMenu[static_cast<int>(Item::pauseSound)].x + kFontSize *6,
		m_pauseMenu[static_cast<int>(Item::pauseSound)].y, m_pauseMenu[static_cast<int>(Item::pauseSound)].y + kFontSize))
	{
		if (m_selectNum != static_cast<int>(Item::pauseSound))
		{
			m_selectNum = static_cast<int>(Item::pauseSound);
			SoundManager::GetInstance().Play(SoundId::Cursor);
		}
		isSelect = true;
	}
	else if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::pauseFullScene)].x, m_pauseMenu[static_cast<int>(Item::pauseFullScene)].x + kFontSize *9,
		m_pauseMenu[static_cast<int>(Item::pauseFullScene)].y, m_pauseMenu[static_cast<int>(Item::pauseFullScene)].y + kFontSize))
	{
		if (m_selectNum != static_cast<int>(Item::pauseFullScene))
		{
			m_selectNum = static_cast<int>(Item::pauseFullScene);
			SoundManager::GetInstance().Play(SoundId::Cursor);
		}
		isSelect = true;
	}
	else if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::pauseBack)].x, m_pauseMenu[static_cast<int>(Item::pauseBack)].x + kFontSize *2,
		m_pauseMenu[static_cast<int>(Item::pauseBack)].y, m_pauseMenu[static_cast<int>(Item::pauseBack)].y + kFontSize))
	{
		if (m_selectNum != static_cast<int>(Item::pauseBack))
		{
			m_selectNum = static_cast<int>(Item::pauseBack);
			SoundManager::GetInstance().Play(SoundId::Cursor);
		}
		isSelect = true;
	}
	else if (mouse.MouseSelect(m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].x, m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].x + kFontSize *5,
		m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].y, m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].y + kFontSize))
	{
		if (m_selectNum != static_cast<int>(Item::pauseGameEnd))
		{
			m_selectNum = static_cast<int>(Item::pauseGameEnd);
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
				m_pauseMenu[i].color = 0xaaffaa;
			}
			else
			{
				m_pauseMenu[i].size = kFontSize;
				m_pauseMenu[i].color = 0xffffff;
			}
		}
	}

	if (input.IsTriggered(InputType::slect))
	{
		switch (m_selectNum)
		{
		case static_cast<int>(Item::pauseSound):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new SoundSettingScene(m_manager, m_soundH));
			return;
		case static_cast<int>(Item::pauseFullScene):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"フルスクリーン変更しますか", SelectType::Scene, m_soundH));
			return;
		case static_cast<int>(Item::pauseBack):
			SoundManager::GetInstance().Play(SoundId::Back);
			m_manager.PopScene();
			return;
		case static_cast<int>(Item::pauseGameEnd):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager,L"ゲームを終了しますか", SelectType::End,m_soundH));
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

void SettingScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//せっていウィンドウセロファン
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//せってい中メッセージ
	SetFontSize(kFontSize * 2);
	DrawString(kPosX, kPosY, L"せってい", 0xffff88);
	for (auto& menu : m_pauseMenu)
	{
		SetFontSize(menu.size);
		DrawString(menu.x, menu.y, menu.name, menu.color);
	}
	SetFontSize(0);
	//せっていウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}
