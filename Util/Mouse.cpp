#include "Mouse.h"
#include <DxLib.h>
#include "DrawFunctions.h"
#include "../game.h"

namespace
{
	constexpr int kRipplesImgWidth = 100;//�g��摜�T�C�YX
	constexpr int kRipplesImgHeight = 100;//�g��摜�T�C�YY
	constexpr float kRipplesDrawScale = 1.0f;//�g��g�嗦
	constexpr int kRipplesAnimNum = 31;//�g��A�j���[�V��������
	constexpr int  kRipplesAnimSpeed = 1;//�g��A�j���[�V�����X�s�[�h
}

Mouse::Mouse() :m_mouseLog(), m_rect{ {},{} }
{
	m_mouseH = my::MyLoadGraph(L"Data/Cursor.png");
	m_ripplesH = my::MyLoadGraph(L"Data/ripples.png");
	MouseReset();
}

Mouse::~Mouse()
{
	DeleteGraph(m_mouseH);
	DeleteGraph(m_ripplesH);
}

void Mouse::Update()
{
	//���O�̍X�V
	for (int i = kLogNum - 1; i >= 1; i--)
	{
		m_mouseLog[i] = m_mouseLog[i - 1];
	}

	//���N���b�N�����Ƃ�
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		m_mouseLog[0] = InputType::left;
		if (IsTrigger(InputType::left))
		{
			ClickAnimCreate();
		}
	}
	//�E�N���b�N�����Ƃ�
	else if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		m_mouseLog[0] = InputType::right;
		if (IsTrigger(InputType::right))
		{
			ClickAnimCreate();
		}
	}
	//���������Ă��Ȃ��Ƃ�
	else
	{
		m_mouseLog[0] = InputType::noPressed;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//�g��̍X�V
	RipplesAnimUpdate();

	//�}�E�X�J�[�\���̈ʒu���擾
	m_rect.center = GetMousePos();
}

void Mouse::Draw()
{
	//�g���\������
	for (auto& ripples : m_ripples)
	{
		//�g���\���ł���Ƃ�
		if (ripples.isDisplay)
		{
			int animNum = static_cast<int>(ripples.idx / kRipplesAnimSpeed);
			if (animNum >= kRipplesAnimNum)
			{
				animNum -= kRipplesAnimNum;
			}
			int imgX = animNum % 6 * kRipplesImgWidth;
			int imgY = animNum / 6 * kRipplesImgHeight;
			my::MyDrawRectRotaGraph(ripples.pos.x, ripples.pos.y, imgX, imgY, kRipplesImgWidth, kRipplesImgHeight, kRipplesDrawScale, 0.0f, m_ripplesH, true, false);
		}
	}

	//�}�E�X�J�[�\����\������
	my::MyDrawGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), m_mouseH, true);

#ifdef _DEBUG
	//�}�E�X�J�[�\���̈ʒu��\��
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0xffffff, L"-----x%3f,y%3f", m_rect.center.x, m_rect.center.y);
#endif
}

void Mouse::MouseReset()
{
	//�}�E�X�J�[�\���𒆐S�Ɉړ�������
	SetMousePoint(Game::kScreenWidth / 2, Game::kScreenHeight / 2);
}

Position2 Mouse::GetMousePos() const
{
	int mouseX = 0;//�}�E�X�J�[�\��x���W
	int mouseY = 0;//�}�E�X�J�[�\��y���W

	//�}�E�X�J�[�\���̈ʒu���擾����֐��̕Ԃ�l��-1�̎�
	if (GetMousePoint(&mouseX, &mouseY) == -1)
	{
		//�G���[����
		return Position2(0, 0);
	}

	return Position2(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

bool Mouse::IsPress(InputType type) const
{
	//�ŐV�̃��O���������A������Ă���
	return (m_mouseLog[0] == type);
}

bool Mouse::IsTrigger(InputType type) const
{
	bool isNow = (m_mouseLog[0] == type);//���݂̏��
	bool isLast = (m_mouseLog[1] == InputType::noPressed);//1�t���[���O�̏��

	return (isNow && isLast);//�������Ă��āA1�t���[���O�ɉ��������Ă��Ȃ��Ƃ������锻��
}

bool Mouse::IsRelase(InputType type) const
{
	bool isNow = (m_mouseLog[0] == InputType::noPressed);//���݂̏��
	bool isLast = (m_mouseLog[1] == type);//1�t���[���O�̏��
	return (isNow && isLast);//�������Ă��āA1�t���[���O������Ă����Ƃ�����������ɂȂ�
}

Rect Mouse::GetRect() const
{
	//�����蔻���Ԃ�
	return m_rect;
}

bool Mouse::MouseSelect(int startX, int endX, int startY, int endY) const
{
	//�J�[�\���̈ʒu�����j���[�ʒu�ɂ��邩�ǂ���	
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//�}�E�X�̈ʒu���͈͓��ɂ���
	return true;
}

bool Mouse::MouseSelect(float startX, float endX, float startY, float endY) const
{
	//�J�[�\���̈ʒu�����j���[�ʒu�ɂ��邩�ǂ���
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//�}�E�X�̈ʒu���͈͓��ɂ���
	return true;
}

void Mouse::ClickAnimCreate()
{
	//�g��쐬
	Ripples ripples;
	ripples.pos = m_rect.center;//���݂̈ʒu
	ripples.isDisplay = true;//�g���\������
	ripples.idx = 0;//��ԍŏ��̊G�̃C���f�b�N�X

	//��Ԍ��ɓ����
	m_ripples.push_back(ripples);
}

void Mouse::RipplesAnimUpdate()
{
	//�g��̍X�V
	for (auto& ripples : m_ripples)
	{
		//���ɕ\���ł��Ȃ��Ȃ��Ă��鎞�͏������Ȃ�
		if (!ripples.isDisplay)	continue;

		//�C���f�b�N�X���A�j���[�V���������ȏ�̂Ƃ�
		if (ripples.idx++ >= kRipplesAnimNum * kRipplesAnimSpeed)
		{
			ripples.idx = 0;
			ripples.isDisplay = false;
		}
	}

	//����Ȃ��Ȃ������̂��폜
	m_ripples.remove_if([](const Ripples click) {
		return !click.isDisplay;
		});
}

