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

	constexpr float kButtonDrawScale = 2.0f;//�g�嗦
	//�v���C���[�O���t�B�b�N
	constexpr int kGraphSizeWidth = 32;		//�T�C�Y
	constexpr int kGraphSizeHeight = 32;	//�T�C�Y
	constexpr float kDrawScale = 2.0f;		//�g�嗦
	constexpr int kFrameSpeed = 10;		//�A�j���[�V�����X�s�[�h
}

ExplanationScene::ExplanationScene(SceneManager& manager,int selectChar) : Scene(manager),
m_updateFunc(&ExplanationScene::FadeInUpdat), m_drawFunc(&ExplanationScene::NormalDraw), m_selectChar(selectChar),
m_frameCount(10)
{
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/charSelect.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);

	m_backH = my::MyLoadGraph(L"Data/UI/Back.png");
	int X = 0, Y = 0;
	GetGraphSize(m_backH, &X, &Y);
	X = static_cast<int>(X * kButtonDrawScale);
	Y = static_cast<int>(Y * kButtonDrawScale);
	m_backRect = { {X / 2.0f,Y / 2.0f},{X,Y} };

	m_fruitH = my::MyLoadGraph(L"Data/PointExp.png");
	m_bgH = my::MyLoadGraph(L"Data/Background/Blue.png");
	m_scroll = 0;

	float addX[3] =
	{
		(640 / 2),
		(640 / 3) / 2,
		(640 / 4) / 2
	};
	float addY = (480 / 3) / 2;

	m_fruitsPos.push_back({ { addX[0], addY }, L"�X�C�J",0xdc143c, 10 });
	m_fruitsPos.push_back({ { addX[1], addY * 3 },L"�����S",0xb22222,5 });
	m_fruitsPos.push_back({ { addX[1] * 5, addY * 3 - 10 }, L"�p�C�i�b�v��",0xff8c00, 8 });
	m_fruitsPos.push_back({ { addX[1] * 3, addY * 3 }, L"�o�i�i",0xffd700, 5 });
	m_fruitsPos.push_back({ { addX[2], addY * 5 }, L"�L�E�C",0x3cb371, 3 });
	m_fruitsPos.push_back({ { addX[2] * 3, addY * 5 }, L"�C�`�S",0xff69b4, 2 });
	m_fruitsPos.push_back({ { addX[2] * 5, addY * 5 }, L"�I�����W",0xffa500, 3 });
	m_fruitsPos.push_back({ { addX[2] * 7, addY * 5 }, L"�`�F���[",0xa52a2a, 2 });
}

ExplanationScene::~ExplanationScene()
{
	DeleteSoundMem(m_BgmH);

	DeleteGraph(m_backH);
	DeleteGraph(m_fruitH);
	DeleteGraph(m_bgH);
	m_fruitsPos.clear();
}

void
ExplanationScene::Update(const InputState& input,  Mouse& mouse)
{
	//�������o�֐��|�C���^���Ăяo���@���Z�q�@->*
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

	DrawString(0, 0, L"����", 0xffffff);

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ExplanationScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	//���ǂ�ǂ񖾂邭�Ȃ�
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &ExplanationScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void ExplanationScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	if (m_frameCount-- <= 0)
	{
		m_frameCount = 0;
	}
	
	if (mouse.GetRect().IsHit(m_backRect) && mouse.IsTriggerLeft())//�߂�{�^�������������O�̕\���ɖ߂�
	{
		if (m_drawFunc == &ExplanationScene::FirstExpDraw)
		{
			m_drawFunc = &ExplanationScene::NormalDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::FirstExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::ThirdExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::FourthExpDraw)
		{
			m_drawFunc = &ExplanationScene::ThirdExpDraw;
			return;
		}
	}
	else if (m_frameCount == 0 && mouse.IsTriggerLeft())//�J�E���g��0�̎����̕\���ɐi
	{
		m_frameCount = 10;
		if (m_drawFunc == &ExplanationScene::NormalDraw)
		{
			m_drawFunc = &ExplanationScene::FirstExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::FirstExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::ThirdExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::ThirdExpDraw)
		{
			m_drawFunc = &ExplanationScene::FourthExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::FourthExpDraw)//�Ō�̕\��
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
		m_manager.ChangeScene(new GameplayingScene(m_manager, m_selectChar));
		return;
	}
}

void ExplanationScene::NormalDraw()
{
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"normal", 0xffffff);
}

void ExplanationScene::FirstExpDraw()
{
	//�����痎���Ă���t���[�c���N���b�N���ď����āA�L�����N�^�ɓ�����Ȃ��悤�ɂ��悤
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"1", 0xffffff);
	DrawString(100, 100, L"�����痎���Ă���t���[�c��", 0xffffff);
	DrawString(100, 120, L"�N���b�N���ď����āA", 0xffffff);
	DrawString(100, 140, L"�L�����N�^�ɓ�����Ȃ��悤�ɂ��悤", 0xffffff);
}

void ExplanationScene::PointExpDraw()
{
	float scale = 4.0f;
	int size = static_cast<int>(32 * scale);
	int halfSize = static_cast<int>(size / 2);//�\������傫���̔���
	int add = 25;//�������G�̏�ɕ\���ł���悤��
	int sud = 32 * 2;//���������ɂ��炷
	int fontsize = kFontSize / 2;

	//�t���[�c�������Ɠ��_�����炦��B
	DrawString(80, 80, L"�t���[�c��������\n���_�����炦��", 0x000000);
	DrawStringF(m_fruitsPos[0].pos.x + halfSize + 50, 80.0f, L"���_�������ق�\nHP������", 0x000000);

	for (auto& fruit : m_fruitsPos)
	{
		int left = static_cast<int>(fruit.pos.x - halfSize);
		int top = static_cast<int>(fruit.pos.y - halfSize);

		my::MyDrawRectRotaGraph(fruit.pos.x, fruit.pos.y, left,top , size, size, 1.0f, 0.0f, m_fruitH, true, false);
		DrawStringF(fruit.pos.x - add, fruit.pos.y - sud, fruit.name, fruit.color);
		SetFontSize(kFontSize);
		DrawFormatStringF(fruit.pos.x - fontsize, fruit.pos.y + 32, fruit.color, L"%d�_", fruit.point);
		SetFontSize(0);
	}
}

void ExplanationScene::ThirdExpDraw()
{
	//�L�����N�^�̃n�[�g�����ׂĂȂ��Ȃ�ƏI��
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"3", 0xffffff);
	DrawString(100, 100, L"�L�����N�^�̃n�[�h�����ׂĂȂ��Ȃ�ƏI��", 0xffffff);
}

void ExplanationScene::FourthExpDraw()
{
	//�L�����N�^�����Ȃ���A���_���W�߂悤
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"4", 0xffffff);
	DrawString(100, 100, L"�L�����N�^�����Ȃ���A���_���W�߂悤!!", 0xffffff);
}
