#include "GameoverScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/DrawFunctions.h"
#include "../Game/Character.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameplayingScene.h"

namespace
{
	 constexpr int kMenuFontSize = 40;//�����̃T�C�Y
	 constexpr int kOriginalPosX = Game::kScreenWidth / 4 + kMenuFontSize * 2;    //���j���[������x�ʒu
	 constexpr int kOriginalPosY = kMenuFontSize + kMenuFontSize;    //���j���[������y�ʒu
}

GameoverScene::GameoverScene(SceneManager& manager, std::shared_ptr<Character> character,int count) :
	Scene(manager), m_updateFunc(&GameoverScene::FadeInUpdat),
	m_drawFunc(&GameoverScene::NormalDraw), m_scroll(0),m_char(character), m_point(count)
{
	m_selectNum = menuGameEnd;

	SelectMenu[menuGameEnd].y = kOriginalPosY;
	SelectMenu[menuRestart].y = kOriginalPosY + kMenuFontSize * 2;
	
	for (int i = 0; i < menuNum; i++)
	{
		if (i == m_selectNum)
		{
			SelectMenu[i].color = 0xffa0aa;//�F��ς���
			SelectMenu[i].size = kMenuFontSize * 2;//�傫����ς���
		}
		else
		{
			SelectMenu[i].color = 0xaaa0ff;//���̐F�ɖ߂�
			SelectMenu[i].size = kMenuFontSize;//�傫����ς���
		}
	}

	//m_BgmH = LoadSoundMem(L"Sound/BGM/shizukanoumi.mp3");
	m_bgH = my::MyLoadGraph(L"Data/Background/Gray.png");
}

GameoverScene::~GameoverScene()
{
	DeleteSoundMem(m_BgmH);
	SoundManager::GetInstance().StopBgm(SoundId::Gameover);
}

void
GameoverScene::Update(const InputState& input,  Mouse& mouse)
{
	(this->*m_updateFunc)(input,mouse);
	//�w�i�ړ�
	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}
}

void
GameoverScene::Draw()
{
	//�w�i
	for (int x = -kBgSize / 2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize / 2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x + m_scroll, y + m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	m_char->Draw();

	DrawFormatString(Game::kScreenWidth / 2, 50, 0x000000, L"%d", m_point);

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameoverScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameoverScene::NormalUpdat;
		m_fadeColor = 0x000000;
		m_fadeValue = 0;
		SoundManager::GetInstance().Play(SoundId::Gameover);
	}
}

void GameoverScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	m_char->Update(false);

	if (m_char->GetIdx() == m_char->GetAnimNum(CharAnimType::DoubleJump))
	{
		m_updateFunc = &GameoverScene::MojiUpdate;
		m_drawFunc = &GameoverScene::MojiDraw;
		ChangeVolumeSoundMem(0, m_BgmH);
		PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
		return;
	}
}

void GameoverScene::MojiUpdate(const InputState& input,  Mouse& mouse)
{
	if (m_soundVolume++ >= SoundManager::GetInstance().GetBGMVolume())
	{
		m_soundVolume = SoundManager::GetInstance().GetBGMVolume();
	}
	ChangeVolumeSoundMem(m_soundVolume, m_BgmH);

	//���j���[
	bool isPress = false;//�L�[�������ꂽ���ǂ���
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % menuNum;//�I����Ԃ��������
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//�I����Ԃ���グ��
		isPress = true;
	}

	//�}�E�X�őI��
	if (mouse.MouseSelect(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].x + kMenuFontSize * 7,
		SelectMenu[menuGameEnd].y, SelectMenu[menuGameEnd].y + kMenuFontSize))
	{
		m_selectNum = static_cast<int>(menuGameEnd);
		isPress = true;
	}
	else if (mouse.MouseSelect(SelectMenu[menuRestart].x, SelectMenu[menuRestart].x + kMenuFontSize * 4,
		SelectMenu[menuRestart].y, SelectMenu[menuRestart].y + kMenuFontSize))
	{
		m_selectNum = static_cast<int>(menuRestart);
		isPress = true;
	}

	if (isPress)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].color = 0xffa0aa;//�F��ς���
				SelectMenu[i].size = kMenuFontSize * 2;//�傫����ς���
			}
			else
			{
				SelectMenu[i].color = 0xaaa0ff;//���̐F�ɖ߂�
				SelectMenu[i].size = kMenuFontSize;//�傫����ς���
			}
		}
	}
	
	//�u���ցv�{�^���������ꂽ�玟�V�[���ֈڍs����
	if (input.IsTriggered(InputType::slect))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);

		m_updateFunc = &GameoverScene::FadeOutUpdat;
	}
}

void GameoverScene::NormalDraw()
{
	SetFontSize(kMenuFontSize);
	DrawString(kOriginalPosX, kOriginalPosY, L"�Q�[���I�[�o�[",0xffffff);
	SetFontSize(0);
}

void GameoverScene::MojiDraw()
{
	SetFontSize(SelectMenu[menuGameEnd].size);
	DrawString(SelectMenu[menuGameEnd].x + 5, SelectMenu[menuGameEnd].y + 5, L"�^�C�g���ɖ߂�", 0x000000);
	DrawString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, L"�^�C�g���ɖ߂�", SelectMenu[menuGameEnd].color);
	SetFontSize(SelectMenu[menuRestart].size);
	DrawString(SelectMenu[menuRestart].x + 5, SelectMenu[menuRestart].y + 5, L"������x", 0x000000);
	DrawString(SelectMenu[menuRestart].x, SelectMenu[menuRestart].y, L"������x", SelectMenu[menuRestart].color);
	SetFontSize(0);
}

void GameoverScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		switch (m_selectNum)
		{
		case menuGameEnd:
			m_manager.ChangeScene(new TitleScene(m_manager));
			return;
		case menuRestart:
			m_manager.ChangeScene(new GameplayingScene(m_manager,m_char->GetSelectChar()));
			return;
		default:
			return;
		}
	}
}

