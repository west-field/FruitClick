#include "SoundSettingScene.h"
#include <Dxlib.h>
#include "../Util/Mouse.h"
#include "../Util/Sound.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"

namespace
{
	constexpr int kWindowWidth = 350;//�E�B���h�E�T�C�Y
	constexpr int kWindowHeight = 250;//�E�B���h�E�T�C�Y
	constexpr int kWindowStartX = (640 - kWindowWidth) / 2;//�E�B���h�E�\���J�n�ʒu
	constexpr int kWindowStartY = (480 - kWindowHeight) / 2;//�E�B���h�E�\���J�n�ʒu

	constexpr int kPosX = kWindowStartX + 20;//�����\���J�n�ʒu
	constexpr int kPosY = kWindowStartY + 30;//�����\���J�n�ʒu

	constexpr int kFontSize = 20;//�t�H���g�T�C�Y
}

SoundSettingScene::SoundSettingScene(SceneManager& manager,int soundH) :Scene(manager), m_soundH(soundH), m_updateFunc(&SoundSettingScene::NormalUpdate)
{
	m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].y = kPosY + kFontSize * (static_cast<int>(SoundItem::soundTypeBGM) + 1) + 10;
	m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].name = L"BGM Volume = %d";

	m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].y = kPosY + kFontSize * (static_cast<int>(SoundItem::soundTypeSE) + 1) + 20;
	m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].name = L"SE Volume = %d";
	
	m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].y = kPosY + kFontSize * (static_cast<int>(SoundItem::soundTypeBack) + 1) + 30;
	m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].name = L"�߂�";
	
	for (auto& sound : m_soundChange)
	{
		sound.x = kPosX;
		sound.fontSize = kFontSize;
		sound.color = 0x000080;
		sound.nameNum = static_cast<int>(wcslen(sound.name));
	}

	m_bg = my::MyLoadGraph(L"Data/panel_Example1.png");
}

SoundSettingScene::~SoundSettingScene()
{
	DeleteGraph(m_bg);
}

void SoundSettingScene::Update(Mouse& mouse)
{
	(this->*m_updateFunc)(mouse);
}

void SoundSettingScene::Draw()
{
	//�E�B���h�E�w�i
	WindowDraw();
	SetFontSize(kFontSize);
	//�T�E���h�Z�b�e�B���O�����b�Z�[�W
	DrawString(kPosX, kWindowStartY + 10, L"�T�E���h�ݒ�", 0x000000);
	//BGM�̐ݒ�
	auto& soundMgr = SoundManager::GetInstance();
	SetFontSize(m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].fontSize);
	DrawFormatString(m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].x, m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].y, 
		m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].color,
		m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].name, soundMgr.GetBGMVolume());
	//SE�̐ݒ�
	SetFontSize(m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].fontSize);
	DrawFormatString(m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].x, m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].y,
		m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].color,
		m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].name, soundMgr.GetSEVolume());

	SetFontSize(m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].fontSize);
	DrawFormatString(m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].x, m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].y,
		m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].color,
		m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].name);
	SetFontSize(0);

#ifdef _DEBUG
	int size = m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].nameNum * (m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].fontSize/2);
	DrawBox(m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].x, m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].y, m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].x + size, m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].y + m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].fontSize, m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].color, false);
#endif
}

void SoundSettingScene::NormalUpdate(Mouse& mouse)
{
	bool isSelect = false;
	m_selectNum = -1;

	//�}�E�X�őI��
	if (mouse.MouseSelect(m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].x,
		m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].x + (m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].fontSize/2) * m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].nameNum, 
		m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].y,m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].y + m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].fontSize))
	{
		if (m_selectNum != static_cast<int>(SoundItem::soundTypeBGM))
		{
			m_selectNum = static_cast<int>(SoundItem::soundTypeBGM);
		}
		isSelect = true;
	}
	else if (mouse.MouseSelect(m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].x,
		m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].x +(m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].fontSize/2) * m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].nameNum,
		m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].y, m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].y + m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].fontSize))
	{
		if (m_selectNum != static_cast<int>(SoundItem::soundTypeSE))
		{
			m_selectNum = static_cast<int>(SoundItem::soundTypeSE);
		}
		isSelect = true;
	}
	else if (mouse.MouseSelect(m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].x,
		m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].x + m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].fontSize* m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].nameNum,
		m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].y, m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].y + m_soundChange[static_cast<int>(SoundItem::soundTypeBack)].fontSize))
	{
		if (m_selectNum != static_cast<int>(SoundItem::soundTypeBack))
		{
			m_selectNum = static_cast<int>(SoundItem::soundTypeBack);
		}
		isSelect = true;
	}

	int num = static_cast<int>(SoundItem::soundTypeMax);
	for (int i = 0; i < num; i++)
	{
		if (i == m_selectNum)
		{
			m_soundChange[i].fontSize = kFontSize + 15;
			m_soundChange[i].color = 0xaaffaa;
		}
		else
		{
			m_soundChange[i].fontSize = kFontSize;
			m_soundChange[i].color = 0x000080;
		}
	}
	auto& soundMgr = SoundManager::GetInstance();

	if (isSelect && mouse.IsTrigger(Mouse::InputType::left))
	{
		switch (m_selectNum)
		{
		case static_cast<int>(SoundItem::soundTypeBGM):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].color = 0xffa000;
			m_updateFunc = &SoundSettingScene::BGMVolumeChange;
			return;
		case static_cast<int>(SoundItem::soundTypeSE):
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].color = 0xffa000;
			m_updateFunc = &SoundSettingScene::SEVolumeChange;
			return;
		case static_cast<int>(SoundItem::soundTypeBack):
			SoundManager::GetInstance().Play(SoundId::Back);
			soundMgr.SaveSoundConfig();
			m_manager.PopScene();
			return;
		default:
			break;
		}
	}
	if (mouse.IsTrigger(Mouse::InputType::right))
	{
		soundMgr.SaveSoundConfig();
		SoundManager::GetInstance().Play(SoundId::Back);
		m_selectNum = static_cast<int>(SoundItem::soundTypeBack);
		m_manager.PopScene();
		return;
	}
}

void SoundSettingScene::BGMVolumeChange(Mouse& mouse)
{
	auto& soundMgr = SoundManager::GetInstance();
	int soundVolume = 0;
	//BGM �}�E�X�̃z�C�[���ŉ��ʂ�ς���
	int Rot = 0;
	Rot += GetMouseWheelRotVol();
	if (Rot != 0)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
	}
	soundVolume = soundMgr.GetBGMVolume() + Rot;
	if (soundVolume >= 255)
	{
		soundVolume = 255;
	}
	else if (soundVolume <= 0)
	{
		soundVolume = 0;
	}
	soundMgr.SetBGMVolume(soundVolume, m_soundH);

	if (mouse.IsTrigger(Mouse::InputType::left) || mouse.IsTrigger(Mouse::InputType::right))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_soundChange[static_cast<int>(SoundItem::soundTypeBGM)].color = 0xaaffaa;
		m_updateFunc = &SoundSettingScene::NormalUpdate;
		return;
	}
}

void SoundSettingScene::SEVolumeChange(Mouse& mouse)
{
	auto& soundMgr = SoundManager::GetInstance();
	int soundVolume = 0;

	//SE �}�E�X�̃z�C�[���ŉ��ʂ�ς���
	int Rot = 0;
	Rot += GetMouseWheelRotVol();
	if (Rot != 0)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
	}
	soundVolume = soundMgr.GetSEVolume() + Rot;
	if (soundVolume >= 255)
	{
		soundVolume = 255;
	}
	else if (soundVolume <= 0)
	{
		soundVolume = 0;
	}
	soundMgr.SetSEVolume(soundVolume);

	if (mouse.IsTrigger(Mouse::InputType::left) || mouse.IsTrigger(Mouse::InputType::right))
	{
		SoundManager::GetInstance().Play(SoundId::Back);
		m_soundChange[static_cast<int>(SoundItem::soundTypeSE)].color = 0xaaffaa;
		m_updateFunc = &SoundSettingScene::NormalUpdate;
		return;
	}
}

void SoundSettingScene::WindowDraw()
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

