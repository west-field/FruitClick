#include "GameendScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../Game/Character.h"
#include "GameplayingScene.h"


namespace
{
	constexpr int kMenuFontSize = 20;//�����̃T�C�Y
	constexpr int kOriginalPosX = Game::kScreenWidth / 4 + kMenuFontSize * 2;    //���j���[������x�ʒu
	constexpr int kOriginalPosY = kMenuFontSize + kMenuFontSize;    //���j���[������y�ʒu

	constexpr int kFontWidth = 16;
	constexpr int kFontHeight = 32;
}

GameendScene::GameendScene(SceneManager& manager, std::shared_ptr<Character> character,int count) :
	Scene(manager), m_updateFunc(&GameendScene::FadeInUpdat), m_drawFunc (&GameendScene::NormalDraw),
	m_char(character), m_scroll(0), m_point(0),m_pointAdd(count), m_pointCount(0)
{
	m_selectNum = menuGameEnd;

	SelectMenu[menuGameEnd].x = kMenuFontSize;
	SelectMenu[menuRestart].x = Game::kScreenWidth / 2 + kMenuFontSize;

	for (int i = 0; i < menuNum; i++)
	{
		SelectMenu[i].y = static_cast<int>(character->GetRect().GetCenter().y);
		if (i == m_selectNum)
		{
			SelectMenu[i].color = 0xffa0aa;//�F��ς���
			SelectMenu[i].fontSize = kMenuFontSize * 2;//�傫����ς���
		}
		else
		{
			SelectMenu[i].color = 0xaaa0ff;//���̐F�ɖ߂�
			SelectMenu[i].fontSize = kMenuFontSize;//�傫����ς���
		}
	}

	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/gameEnd.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	m_bgH = my::MyLoadGraph(L"Data/Background/Brown.png");
	m_numFont = my::MyLoadGraph(L"Data/numfont.png");
}

GameendScene::~GameendScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_bgH);
	DeleteGraph(m_numFont);
}

void GameendScene::Update(const InputState& input,  Mouse& mouse)
{
	m_char->Update(true);
	(this->*m_updateFunc)(input,mouse);
	//�w�i�ړ�
	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}
}

void GameendScene::Draw()
{
	//�w�i
	for (int x = -kBgSize / 2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize / 2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x , y + m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	m_char->Draw(false);

	PointUpdate(Game::kScreenWidth / 2, Game::kScreenHeight / 2, m_point);

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameendScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameendScene::NormalUpdat;
		m_fadeValue = 0;
	}
}
void GameendScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
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
			m_manager.ChangeScene(new GameplayingScene(m_manager, m_char->GetSelectChar()));
			return;
		default:
			return;
		}
	}
}

void GameendScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	if (m_pointCount-- <= 0)
	{
		if (m_pointAdd != 0)
		{
			m_pointAdd--;
			m_point++;
			SoundManager::GetInstance().Play(SoundId::Point);
		}
		m_pointCount = 3;
	}
	if (m_pointAdd != 0 && input.IsTriggered(InputType::slect))
	{
		m_point += m_pointAdd;
		m_pointAdd = 0;
	}

	if (m_pointAdd == 0)
	{
		m_updateFunc = &GameendScene::MojiUpdate;
		m_drawFunc = &GameendScene::MojiDraw;
		return;
	}
}

void GameendScene::MojiUpdate(const InputState& input, Mouse& mouse)
{
	//���j���[
	bool isPress = false;//�L�[�������ꂽ���ǂ���
	if (input.IsTriggered(InputType::down)|| input.IsTriggered(InputType::right))
	{
		m_selectNum = (m_selectNum + 1) % menuNum;//�I����Ԃ��������
		SoundManager::GetInstance().Play(SoundId::Cursor);
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up)|| input.IsTriggered(InputType::left))
	{
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//�I����Ԃ���グ��
		SoundManager::GetInstance().Play(SoundId::Cursor);
		isPress = true;
	}

	//�}�E�X�őI��
	if (mouse.MouseSelect(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].x + kMenuFontSize * 7,
		SelectMenu[menuGameEnd].y, SelectMenu[menuGameEnd].y + kMenuFontSize))
	{
		if (m_selectNum != static_cast<int>(menuGameEnd))
		{
			m_selectNum = static_cast<int>(menuGameEnd);
			SoundManager::GetInstance().Play(SoundId::Cursor);
		}
		isPress = true;
	}
	else if (mouse.MouseSelect(SelectMenu[menuRestart].x, SelectMenu[menuRestart].x + kMenuFontSize * 4,
		SelectMenu[menuRestart].y, SelectMenu[menuRestart].y + kMenuFontSize))
	{
		if (m_selectNum != static_cast<int>(menuRestart))
		{
			m_selectNum = static_cast<int>(menuRestart);
			SoundManager::GetInstance().Play(SoundId::Cursor);
		}
		isPress = true;
	}

	if (isPress)
	{
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].color = 0xffa0aa;//�F��ς���
				SelectMenu[i].fontSize = kMenuFontSize * 2;//�傫����ς���
			}
			else
			{
				SelectMenu[i].color = 0xaaa0ff;//���̐F�ɖ߂�
				SelectMenu[i].fontSize = kMenuFontSize;//�傫����ς���
			}
		}
	}

	//�u���ցv�{�^���������ꂽ�玟�V�[���ֈڍs����
	if (input.IsTriggered(InputType::slect))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);

		m_updateFunc = &GameendScene::FadeOutUpdat;
	}
}

void GameendScene::NormalDraw()
{
#ifdef _DEBUG
	DrawString(0, Game::kScreenHeight / 2, L"�Q�[���N���A", 0xffffff);
#endif
}

void GameendScene::MojiDraw()
{
	SetFontSize(SelectMenu[menuGameEnd].fontSize);
	DrawString(SelectMenu[menuGameEnd].x + 5, SelectMenu[menuGameEnd].y + 5, L"�^�C�g���ɖ߂�", 0x000000);
	DrawString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, L"�^�C�g���ɖ߂�", SelectMenu[menuGameEnd].color);
	SetFontSize(SelectMenu[menuRestart].fontSize);
	DrawString(SelectMenu[menuRestart].x + 5, SelectMenu[menuRestart].y + 5, L"������x", 0x000000);
	DrawString(SelectMenu[menuRestart].x, SelectMenu[menuRestart].y, L"������x", SelectMenu[menuRestart].color);
	SetFontSize(0);
}

void GameendScene::PointUpdate(int leftX, int y, int dispNum, int digit)
{
	int digitNum = 0;
	int temp = dispNum;
	int cutNum = 10;	// �\�������w�莞�ɕ\���������߂邽�߂Ɏg�p����

	// �\�����鐔���̌���������
	while (temp > 0)
	{
		digitNum++;
		temp /= 10;
		cutNum *= 10;
	}
	if (digitNum <= 0)
	{
		digitNum = 1;	// 0�̏ꍇ��1���\������
	}

	// �\�������w��
	temp = dispNum;
	if (digit >= 0)
	{
		if (digitNum > digit)
		{
			// ������w�茅����\�����邽�߂͂ݏo���͈͂�؂�̂�
			temp %= cutNum;
		}
		digitNum = digit;
	}
	// ��ԉ��̌�����\��
	int posX = leftX - static_cast<int>( kFontWidth * 1.5f);
	int posY = y;
	for (int i = 0; i < digitNum; i++)
	{
		int no = temp % 10;

		my::MyDrawRectRotaGraph(posX, posY,
			no * kFontWidth, 0, kFontWidth, kFontHeight, 1.5f, 0.0f, m_numFont, true, false);

		temp /= 10;
		posX -= static_cast<int>(kFontWidth * 1.5f);
	}
}
