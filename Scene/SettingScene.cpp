#include "SettingScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/Sound.h"
#include "../Util/DrawFunctions.h"
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
	m_selectNum = -1;

	m_pauseMenu[static_cast<int>(Item::pauseSound)].x = kPosX +10;
	m_pauseMenu[static_cast<int>(Item::pauseSound)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseSound) + 1) + 10;
	m_pauseMenu[static_cast<int>(Item::pauseSound)].name = L"���ʂ����Ă�";
	
	m_pauseMenu[static_cast<int>(Item::pauseFullScene)].x = kPosX +10;
	m_pauseMenu[static_cast<int>(Item::pauseFullScene)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseFullScene) + 1) + 20;
	m_pauseMenu[static_cast<int>(Item::pauseFullScene)].name = L"�X�N���[�����[�h�ύX";

	m_pauseMenu[static_cast<int>(Item::pauseBack)].x = kPosX +10;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseBack) + 1) + 30;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].name = L"����";

	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].x = kPosX+10;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseGameEnd) + 1) + 40;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].name = L"�Q�[���I��";

	int pauseMax = static_cast<int>(Item::pauseMax);
	for (int i = 0; i < pauseMax; i++)
	{
		m_pauseMenu[i].fontSize = kFontSize;
		m_pauseMenu[i].color = 0x808080;
		m_pauseMenu[i].nameNum = static_cast<int>(wcslen(m_pauseMenu[i].name));
	}
	m_bg = my::MyLoadGraph(L"Data/panel_Example3.png");
}

SettingScene::~SettingScene()
{
	DeleteGraph(m_bg);
}

void SettingScene::Update(const InputState& input, Mouse& mouse)
{
	bool isSelect = false;
	int pauseMax = static_cast<int>(Item::pauseMax);
	m_selectNum = -1;

	//�}�E�X�őI��
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
			m_pauseMenu[i].fontSize = kFontSize + 15;
			m_pauseMenu[i].color = 0xaaffaa;
		}
		else
		{
			m_pauseMenu[i].fontSize = kFontSize;
			m_pauseMenu[i].color = 0x000080;
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
		case static_cast<int>(Item::pauseFullScene):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"�X�N���[�����[�h�ύX���܂���", SelectType::SceneMode, m_soundH));
			return;
		case static_cast<int>(Item::pauseBack):
			SoundManager::GetInstance().Play(SoundId::Back);
			m_manager.PopScene();
			return;
		case static_cast<int>(Item::pauseGameEnd):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"�Q�[�����I�����܂���", SelectType::End, m_soundH));
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
	//�E�B���h�E�\��
	WindowDraw();

	//�����Ă������b�Z�[�W
	SetFontSize(kFontSize * 2);
	DrawString(kPosX + (kFontSize * 2) * 3, kPosY, L"�����Ă�", 0x000000);
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

void SettingScene::WindowDraw()
{
	//�p�\��
	int x = pw_start_x - 50 / 2;
	int y = pw_start_y - 50 / 2;
	DrawRectGraph(x, y,
		0, 0, 50, 50, m_bg, true);//����@50 y3,x9
	DrawRectGraph(x + pw_width, y,
		50 * 8, 0, 50, 50, m_bg, true);//�E��
	DrawRectGraph(x, y + pw_height,
		0, 50 * 2, 50, 50, m_bg, true);//�����@50 y3,x9
	DrawRectGraph(x + pw_width, y + pw_height,
		50 * 8, 50 * 2, 50, 50, m_bg, true);//�E��

	//�摜�̍���A�E���A�O���t�B�b�N�̍��ォ��X�T�C�Y�AY�T�C�Y�A�\������摜�A����
	DrawRectExtendGraph(x + 50, y, x + pw_width, y + 50,
		50 * 2, 0, 50, 50, m_bg, true);//��
	DrawRectExtendGraph(x, y + 50, x + 50, y + pw_height,
		0, 50 * 1, 50, 50, m_bg, true);//��
	DrawRectExtendGraph(x + pw_width, y + 50, x + pw_width + 50, y + pw_height,
		50 * 8, 50, 50, 50, m_bg, true);// �E
	DrawRectExtendGraph(x + 50, y + pw_height, x + pw_width, y + pw_height + 50,
		50 * 2, 50 * 2, 50, 50, m_bg, true);	// ��
	DrawRectExtendGraph(x + 50, y + 50, x + pw_width, y + pw_height,
		50 * 3, 50 * 1, 50, 50, m_bg, true);	// �E�C���h�E����
}
