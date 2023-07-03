#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/Info.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Mouse.h"

#include "SceneManager.h"
#include "CharacterSelectScene.h"
#include "SettingScene.h"

namespace
{
	constexpr int kMenuFontSize = 50;//�����̃T�C�Y
	constexpr float kSettingDrawScale = 2.0f;//�ݒ�摜�\���g�嗦

	constexpr int kTextNumX = 10;
	constexpr int kTextNumY = 5 - 1;

	constexpr int kTextSizeW = 8;
	constexpr int kTextSizeH = 10;
}


TitleScene::TitleScene(SceneManager& manager) : Scene(manager),
	m_updateFunc(&TitleScene::FadeInUpdat), m_selectNum(0), m_instrString(0),
	m_moveAdd(1), m_settingH(-1),m_scroll(0)
{
	//BGM
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/title.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	//�ݒ�O���t�B�b�N
	m_settingH = my::MyLoadGraph(L"Data/UI/Settings.png");
	int X = 0, Y = 0;
	GetGraphSize(m_settingH, &X, &Y);//�T�C�Y�擾
	X = static_cast<int>(X * kSettingDrawScale);//�摜���g��
	Y = static_cast<int>(Y * kSettingDrawScale);
	m_settingRect = { {static_cast<float>(Game::kScreenWidth - X / 2),static_cast<float>(Y/2)}, {X,Y} };//�ݒ�摜��`
	//�w�i
	m_bgH = my::MyLoadGraph(L"Data/Background/Yellow.png");

	//�L�����N�^�[
	m_charPos = {0.0f,static_cast<float>(Game::kScreenHeight - 100) };//�ʒu
	m_char[0] = my::MyLoadGraph(L"Data/Characters/MaskDude/Run.png");//�L�����N�^�[
	m_char[1] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Run.png");
	m_char[2] = my::MyLoadGraph(L"Data/Characters/PinkMan/Run.png");
	m_char[3] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Run.png");
	m_charType = 0;//�L�����N�^�[�^�C�v
	m_frameCount = 0;//�摜�ύX
	m_idx = 0;//�A�j���[�V����
}

TitleScene::~TitleScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_settingH);
	DeleteGraph(m_bgH);
	for (auto& chartype : m_char)
	{
		DeleteGraph(chartype);
	}
}

void
TitleScene::Update(Mouse& mouse)
{
	//�������o�֐��|�C���^���Ăяo���@���Z�q�@->*
	(this->*m_updateFunc)(mouse);

	if (m_scroll++ >= static_cast<int>(kBgSize ))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}

	m_charPos.x += 2.0f;//�L�����N�^�[�ړ�
	//�L�����N�^�[����ʉE�[�܂Ō�������
	if (m_charPos.x >= Game::kScreenWidth + 32 / 2)
	{
		m_charPos.x = -32 / 2;//���[�֖߂�
		m_charType = GetRand(3);//���ɕ\������L�����N�^�[�^�C�v�������_���Ɍ��߂�
	}

	//�L�����N�^�[�A�j���[�V����
	if (m_frameCount-- <= 0)
	{
		if (m_idx++ >= 12 - 1)
		{
			m_idx -= 12;
		}
		m_frameCount = 5;
	}
}

void TitleScene::Draw()
{
	//�w�i
	for (int x = -kBgSize/2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize/2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x, y+ m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}
	//�L�����N�^�[
	for (int i = 0; i < 4; i++)
	{
		if (i != m_charType)	continue;
		my::MyDrawRectRotaGraph(m_charPos.x, m_charPos.y, m_idx * 32, 0, 32, 32, 2.0f, 0.0f, m_char[m_charType], true, false);
		break;
	}
	//������\��
	SetFontSize(kMenuFontSize);
	DrawString( (Game::kScreenWidth - 13 * kMenuFontSize) /2, (Game::kScreenHeight - kMenuFontSize) / 2 + m_instrString, L"�N���b�N���ăQ�[���X�^�[�g", 0x00fff0);
	SetFontSize(kMenuFontSize+10);
	DrawString( 100, 100, L"Fruits Click", 0x00ff00);
	SetFontSize(0);

	//���j���[�{�b�N�X��`��
	my::MyDrawRectRotaGraph(m_settingRect.center.x, m_settingRect.center.y,
		0, 0, m_settingRect.size.w, m_settingRect.size.h,
		kSettingDrawScale, 0.0f, m_settingH, true, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::FadeInUpdat(Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);//��ʂ̃t�F�[�h�C��
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);//���ʂ̃t�F�[�h�C��
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat( Mouse& mouse)
{
	//���ɂ���w����������c�ɗh�炷
	m_instrString += m_moveAdd;
	if (m_instrString >= 60 || m_instrString <= 0)
	{
		m_moveAdd *= -1;
	}
	//�}�E�X�J�[�\�����ݒ��`�Əd�Ȃ��Ă�����
	if (mouse.GetRect().IsHit(m_settingRect))
	{
		m_selectNum = static_cast<int>(MenuItem::menuConfig);//�ݒ��I��
	}
	else
	{
		m_selectNum = static_cast<int>(MenuItem::menuGameStart);//�Q�[���X�^�[�g��I��
	}

	//�}�E�X�������ꂽ�玟�V�[���ֈڍs����
	if(mouse.IsTriggerLeft())
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		if (m_selectNum == static_cast<int>(MenuItem::menuConfig))
		{
			m_manager.PushScene(new SettingScene(m_manager,m_BgmH));
			return;
		}
		else if(m_selectNum == static_cast<int>(MenuItem::menuGameStart))
		{
			m_updateFunc = &TitleScene::FadeOutUpdat;
		}
	}
	//�E�N���b�N�����Ƃ��ݒ��ʂ�\������
	if (mouse.IsTriggerRight())
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_manager.PushScene(new SettingScene(m_manager, m_BgmH));
		return;
	}
}

void TitleScene::FadeOutUpdat( Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new CharacterSelectScene(m_manager));
		return;
	}
}

