#include "GameendScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/Mouse.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameplayingScene.h"
#include "../Game/Character.h"
#include "../Game/Score.h"

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
	m_selectNum = -1;//�I�����Ă��郁�j���[
	//���j���[
	m_selectMenu[menuGameEnd].x = kMenuFontSize;
	m_selectMenu[menuGameEnd].name = L"�^�C�g���ɖ߂�";
	m_selectMenu[menuRestart].x = Game::kScreenWidth / 2 + kMenuFontSize;
	m_selectMenu[menuRestart].name = L"������x";

	for (int i = 0; i < menuNum; i++)
	{
		m_selectMenu[i].y = static_cast<int>(character->GetRect().GetCenter().y);
		m_selectMenu[i].color = 0xaaa0ff;
		m_selectMenu[i].fontSize = kMenuFontSize;
		m_selectMenu[i].nameNum = static_cast<int>(wcslen(m_selectMenu[i].name));
	}

	//BGM
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/gameEnd.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	//�w�i
	m_bgH = my::MyLoadGraph(L"Data/Background/Brown.png");
	//����
	m_numFont = my::MyLoadGraph(L"Data/numfont.png");

	//���_
	m_score = std::make_shared<Score>();
	m_score->Load();
}

GameendScene::~GameendScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_bgH);
	DeleteGraph(m_numFont);
}

void GameendScene::Update( Mouse& mouse)
{
	m_char->Update(true);
	(this->*m_updateFunc)(mouse);
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
	//�L�����N�^�[
	m_char->Draw(false);

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameendScene::FadeInUpdat( Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameendScene::NormalUpdat;
		m_fadeValue = 0;
	}
}
void GameendScene::FadeOutUpdat( Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		switch (m_selectNum)
		{
		case menuGameEnd:
			m_manager.ChangeScene(new TitleScene(m_manager));//�^�C�g��
			return;
		case menuRestart:
			m_manager.ChangeScene(new GameplayingScene(m_manager, m_char->GetSelectChar()));//�Q�[���v���C���O�V�[��
			return;
		default:
			return;
		}
	}
}

void GameendScene::NormalUpdat( Mouse& mouse)
{
	//�J�E���g�A�b�v
	m_pointCount--;
	if (m_pointCount <= 0)
	{
		if (m_pointAdd != 0)
		{
			m_pointAdd--;
			m_point++;
			SoundManager::GetInstance().Play(SoundId::Point);//�|�C���g�擾��
			m_pointCount = 3;
		}
		//�Ō�܂ōs������
		else if (m_pointAdd == 0)
		{
			m_score->Comparison(m_point);
			m_updateFunc = &GameendScene::RankUpdate;
			m_drawFunc = &GameendScene::RankDraw;
			return;
		}
	}
	//�|�C���g�擾�r���ŃN���b�N������A�J�E���g�A�b�v���I��点��
	if (m_pointAdd != 0 && mouse.IsTriggerLeft())
	{
		m_point += m_pointAdd;
		m_pointAdd = 0;
		m_pointCount = 60;
	}
}

void GameendScene::RankUpdate(Mouse& mouse)
{
	//���j���[
	bool isSelect = false;//�L�[�������ꂽ���ǂ���
	m_selectNum = -1;
	//�}�E�X�őI��
	int i = 0;
	for (auto& menu : m_selectMenu)
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
	
	for (int i = 0; i < menuNum; i++)
	{
		if (i == m_selectNum)
		{
			m_selectMenu[i].color = 0x191970;//�F��ς���
			m_selectMenu[i].fontSize = kMenuFontSize * 2;//�傫����ς���
		}
		else
		{
			m_selectMenu[i].color = 0xaaa0ff;//���̐F�ɖ߂�
			m_selectMenu[i].fontSize = kMenuFontSize;//�傫����ς���
		}
	}

	//�N���b�N�����玟�V�[���ֈڍs����
	if (isSelect && mouse.IsTriggerLeft())
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);

		m_updateFunc = &GameendScene::FadeOutUpdat;
	}
}

void GameendScene::NormalDraw()
{
	//�|�C���g��\��
	PointUpdate(Game::kScreenWidth / 2, Game::kScreenHeight / 2, m_point);
}

void GameendScene::RankDraw()
{
	m_score->Draw();//�����L���O��\��
	//���j���[��\��
	for (auto& menu : m_selectMenu)
	{
		SetFontSize(menu.fontSize);
		DrawString(menu.x, menu.y, menu.name, menu.color);
	}
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
