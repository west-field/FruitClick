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
	m_pauseMenu[static_cast<int>(Item::pauseSound)].name = L"���ʂ����Ă�";

	m_pauseMenu[static_cast<int>(Item::pauseFullscreen)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseFullscreen)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseFullscreen) + 1) + 20;
	m_pauseMenu[static_cast<int>(Item::pauseFullscreen)].name = L"�X�N���[�����[�h�ύX";

	m_pauseMenu[static_cast<int>(Item::pauseBack)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseBack) + 1) + 30;
	m_pauseMenu[static_cast<int>(Item::pauseBack)].name = L"����";

	m_pauseMenu[static_cast<int>(Item::pauseTitle)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseTitle)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseTitle) + 1) + 40;
	m_pauseMenu[static_cast<int>(Item::pauseTitle)].name = L"�^�C�g���߂�";

	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].x = kPosX + 10;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].y = kPosY + kFontSize * 2 * (static_cast<int>(Item::pauseGameEnd) + 1) + 50;
	m_pauseMenu[static_cast<int>(Item::pauseGameEnd)].name = L"�Q�[���I��";

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

	//�I��
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
			m_manager.PushScene(new ConfirmationScene(m_manager, L"�X�N���[�����[�h�ύX���܂���", SelectType::SceneMode, m_soundH));
			return;
		case static_cast<int>(Item::pauseBack):
			SoundManager::GetInstance().Play(SoundId::Back);
			m_manager.PopScene();
			return;
		case static_cast<int>(Item::pauseTitle):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"�^�C�g���ɖ߂�܂���", SelectType::BackTitle, m_soundH));
			return;
		case static_cast<int>(Item::pauseGameEnd):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_manager.PushScene(new ConfirmationScene(m_manager, L"�Q�[�����I�����܂���", SelectType::End, m_soundH));
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
	//�E�B���h�E�w�i
	WindowDraw();
	//�|�[�Y�����b�Z�[�W
	SetFontSize(kFontSize * 2);

	DrawString(kWindowStartX + 10+ (kFontSize * 2) * 3, kWindowStartY + 10, L"�|�[�Y", 0x000000);
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
	//�p�\��
	int x = kWindowStartX - 50 / 2;
	int y = kWindowStartY - 50 / 2;
	DrawRectGraph(x, y,
		0, 0, 50, 50, m_bg, true);//����@50 y3,x9
	DrawRectGraph(x + kWindowWidth, y,
		50 * 8, 0, 50, 50, m_bg, true);//�E��
	DrawRectGraph(x, y + kWindowHeight,
		0, 50 * 2, 50, 50, m_bg, true);//�����@50 y3,x9
	DrawRectGraph(x + kWindowWidth, y + kWindowHeight,
		50 * 8, 50 * 2, 50, 50, m_bg, true);//�E��

	//�摜�̍���A�E���A�O���t�B�b�N�̍��ォ��X�T�C�Y�AY�T�C�Y�A�\������摜�A����
	DrawRectExtendGraph(x + 50, y, x + kWindowWidth, y + 50,
		50 * 2, 0, 50, 50, m_bg, true);//��
	DrawRectExtendGraph(x, y + 50, x + 50, y + kWindowHeight,
		0, 50 * 1, 50, 50, m_bg, true);//��
	DrawRectExtendGraph(x + kWindowWidth, y + 50, x + kWindowWidth + 50, y + kWindowHeight,
		50 * 8, 50, 50, 50, m_bg, true);// �E
	DrawRectExtendGraph(x + 50, y + kWindowHeight, x + kWindowWidth, y + kWindowHeight + 50,
		50 * 2, 50 * 2, 50, 50, m_bg, true);	// ��
	DrawRectExtendGraph(x + 50, y + 50, x + kWindowWidth, y + kWindowHeight,
		50 * 3, 50 * 1, 50, 50, m_bg, true);	// �E�C���h�E����
}
