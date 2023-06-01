#include "ExplanationScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/Info.h"
#include "../Util/DrawFunctions.h"

#include "SceneManager.h"
#include "GameplayingScene.h"

namespace
{
	constexpr int kFontSize = 30;//�t�H���g�T�C�Y

	constexpr float kButtonDrawScale = 2.0f;//�߂�{�^���g�嗦

	constexpr float kFruitsDrawScale = 4.0f;//�t���[�c�g�嗦
	constexpr int kFruitsSize= 32;		//�t���[�c�摜�T�C�Y
	constexpr int kFruitsDrawSize = static_cast<int>(kFruitsSize * kFruitsDrawScale);//�t���[�c��ʕ\���T�C�Y
}

ExplanationScene::ExplanationScene(SceneManager& manager,int selectChar) : Scene(manager),
m_updateFunc(&ExplanationScene::FadeInUpdat), m_drawFunc(&ExplanationScene::RuleExpDraw), m_selectChar(selectChar),
m_frameCount(10)
{
	//BGM
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/charSelect.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);

	//�߂�{�^��
	m_backH = my::MyLoadGraph(L"Data/UI/Back.png");
	int X = 0, Y = 0;
	GetGraphSize(m_backH, &X, &Y);
	X = static_cast<int>(X * kButtonDrawScale);
	Y = static_cast<int>(Y * kButtonDrawScale);
	m_backRect = { {X / 2.0f,Y / 2.0f},{X,Y} };

	//�w�i
	m_bgH = my::MyLoadGraph(L"Data/Background/Blue.png");
	m_scroll = 0;

	//�����V�[�����Ɏg�p����摜
	m_hitExpH[0] = my::MyLoadGraph(L"Data/atc1.jpg");
	m_hitExpH[1] = my::MyLoadGraph(L"Data/click1.jpg");
	m_hitExpH[2] = my::MyLoadGraph(L"Data/click2.jpg");
	m_pointExpH = my::MyLoadGraph(L"Data/point.png");
	m_endExpH[0] = my::MyLoadGraph(L"Data/heat1.jpg");
	m_endExpH[1] = my::MyLoadGraph(L"Data/heat2.jpg");
	m_endExpH[2] = my::MyLoadGraph(L"Data/heat3.jpg");
	m_closeExpH = my::MyLoadGraph(L"Data/rule1.png");

	SetFontSize(kFontSize);//�t�H���g�T�C�Y�ύX
}

ExplanationScene::~ExplanationScene()
{
	DeleteSoundMem(m_BgmH);

	DeleteGraph(m_backH);
	DeleteGraph(m_bgH);

	DeleteGraph(m_hitExpH[0]);
	DeleteGraph(m_hitExpH[1]);
	DeleteGraph(m_hitExpH[2]);
	DeleteGraph(m_pointExpH);
	DeleteGraph(m_endExpH[0]);
	DeleteGraph(m_endExpH[1]);
	DeleteGraph(m_endExpH[2]);
	DeleteGraph(m_closeExpH);
}

void
ExplanationScene::Update(const InputState& input,  Mouse& mouse)
{
	(this->*m_updateFunc)(input,mouse);

	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}
}

void ExplanationScene::Draw()
{
	//�w�i
	for (int x = -kBgSize / 2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize / 2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x + m_scroll, y , 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	//�߂�{�^����\��
	my::MyDrawRectRotaGraph(m_backRect.center.x, m_backRect.center.y,
		0, 0, m_backRect.size.w, m_backRect.size.h, kButtonDrawScale, 0.0f, m_backH, true, false);

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ExplanationScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);//�摜�̃t�F�[�h�C��
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);//���̃t�F�[�h�C��
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &ExplanationScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void ExplanationScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	//��莞�Ԃ͂��̉�ʂŎ~�܂�
	if (m_frameCount-- <= 0)
	{
		m_frameCount = 0;
	}
	
	//�߂�{�^�������������O�̕\���ɖ߂�
	if (mouse.GetRect().IsHit(m_backRect) && mouse.IsTriggerLeft())
	{
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::RuleExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::EndExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::CloseDraw)
		{
			m_drawFunc = &ExplanationScene::EndExpDraw;
			return;
		}
	}
	//�J�E���g��0�̎����̕\���ɐi�߂�
	else if (m_frameCount == 0 && mouse.IsTriggerLeft())
	{
		m_frameCount = 10;
		if (m_drawFunc == &ExplanationScene::RuleExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::EndExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::EndExpDraw)
		{
			m_drawFunc = &ExplanationScene::CloseDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::CloseDraw)//�Ō�̕\��
		{
			m_updateFunc = &ExplanationScene::FadeOutUpdat;//���̃V�[���Ɉڍs����
			return;
		}
	}
	
}

void ExplanationScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		SetFontSize(0);
		m_manager.ChangeScene(new GameplayingScene(m_manager, m_selectChar));
		return;
	}
}

//���[������
void ExplanationScene::RuleExpDraw()
{
	DrawString(Game::kScreenWidth / 4 + 75, 100, L"�����痎���Ă���t���[�c��\n�N���b�N���ď�����", 0x000000);
	DrawString(90, Game::kScreenHeight - 110 , L"�L�����N�^�[��\n������Ȃ��悤�ɂ��悤", 0x000000);
	
	int X = Game::kScreenWidth / 4;//�����ʒuX
	int Y = Game::kScreenHeight / 4;//�����ʒuY

	my::MyDrawRectRotaGraph(X, Y, 0, 0, 115, 122, 1.0f, 0.0f, m_hitExpH[1], true, false);//�N���b�N1
	Y += 122 / 2;
	DrawString(X, Y, L"��", 0x000000);
	Y += 122 / 2 + kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 0, 0, 115, 122, 1.0f, 0.0f, m_hitExpH[2], true, false);//�N���b�N2
	X *= 3;
	Y += kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 0, 0, 115, 211, 1.0f, 0.0f, m_hitExpH[0], true, false);//�L�����N�^
}

//�|�C���g����
void ExplanationScene::PointExpDraw()
{
	my::MyDrawRectRotaGraph(642 / 2, 481 / 2, 0, 0, 606, 476, 0.9f, 0.0f, m_pointExpH, true, false);//�����摜��\��
	DrawString(20, 80, L"�t���[�c��������\n���_�����炦��", 0x000000);//������
	DrawStringF((640 / 2) + (static_cast<int>(kFruitsDrawSize / 2)) + 30, 80.0f, L"���_�������ق�\nHP������", 0x000000);
}

//�I���̐���
void ExplanationScene::EndExpDraw()
{
	DrawString(70, 50, L"�L�����N�^�̃n�[�g��\n�@�@�@�@���ׂĂȂ��Ȃ�ƏI��", 0x000000);
	int X = 180;
	int Y = Game::kScreenHeight / 3;

	my::MyDrawRectRotaGraph(X, Y, 10, 10, 339, 77, 1.0f, 0.0f, m_endExpH[0], true, false);
	Y += 77/2;
	DrawString(X, Y, L"��", 0x000000);
	X += 339 / 3;
	Y += 77 / 2 + kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 10, 10, 339, 77, 1.0f, 0.0f, m_endExpH[1], true, false);
	Y += 77/2;
	DrawString(X, Y, L"��", 0x000000);
	X += 339 / 3;
	Y += 77 / 2 + kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 10, 10, 339, 77, 1.0f, 0.0f, m_endExpH[2], true, false);
}

//����
void ExplanationScene::CloseDraw()
{
	my::MyDrawRectRotaGraph(642 / 2, 481 / 2 + kFontSize, 1, 0, 640, 481, 0.7f, 0.0f, m_closeExpH, true, false);
	DrawString(40, 40, L"�L�����N�^�����Ȃ���A���_���҂���!!", 0x000000);
}
