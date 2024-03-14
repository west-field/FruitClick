#include "Mouse.h"
#include <DxLib.h>
#include "DrawFunctions.h"
#include "../game.h"

namespace
{
	constexpr int kClickImgWidth = 100;//�摜�T�C�YX
	constexpr int kClickImgHeight = 100;//�摜�T�C�YY
	constexpr float kClickDrawScale = 1.0f;//�g�嗦
	constexpr int kClickAnimNum = 31;//�A�j���[�V��������
	constexpr int  kClickAnimSpeed = 1;//�A�j���[�V�����X�s�[�h
}

Mouse::Mouse() :mouseLog(), m_rect{ {},{} }
{
	m_rect = { {},{} };
	m_mouseH = my::MyLoadGraph(L"Data/Cursor.png");
	m_ripplesH = my::MyLoadGraph(L"Data/ripples.png");
	MouseReset();
}

Mouse::~Mouse()
{
	DeleteGraph(m_mouseH);
}

void Mouse::Update()
{
	//���O�̍X�V
	for (int i = kLogNum - 1; i >= 1; i--)
	{
		mouseLog[i] = mouseLog[i - 1];
	}

	//�ŐV�̏��
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		mouseLog[0] = 1;
		if (IsTriggerLeft())
		{
			ClickAnimCreate();
		}
	}
	else if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		mouseLog[0] = 2;
		if (IsTriggerRight())
		{
			ClickAnimCreate();
		}
	}
	else
	{
		mouseLog[0] = 0;
	}

	RipplesAnimUpdate();


	m_rect.center = GetPos();
}

void Mouse::Draw()
{
	//�g���\������
	for (auto& ripples : m_ripples)
	{
		//�g���\���ł���Ƃ�
		if (ripples.isDisplay)
		{
			int animNum = static_cast<int>(ripples.idx / kClickAnimSpeed);
			if (animNum >= kClickAnimNum)
			{
				animNum -= kClickAnimNum;
			}
			int imgX = animNum % 6 * kClickImgWidth;
			int imgY = animNum / 6 * kClickImgHeight;
			my::MyDrawRectRotaGraph(ripples.pos.x, ripples.pos.y, imgX, imgY, kClickImgWidth, kClickImgHeight, kClickDrawScale, 0.0f, m_ripplesH, true, false);
		}
	}
	my::MyDrawGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), m_mouseH, true);
#ifdef _DEBUG
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0xffffff, L"-----x%3f,y%3f", m_rect.center.x, m_rect.center.y);
#endif
}

void Mouse::MouseReset()
{
	SetMousePoint(Game::kScreenWidth / 2, Game::kScreenHeight / 2);//�|�C���^�𒆐S�Ɉړ�������
}

Position2 Mouse::GetPos() const
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

bool Mouse::IsPressLeft()
{
	//�ŐV�̃��O��1�̎�������Ă���
	return (mouseLog[0] == 1);
}

bool Mouse::IsPressRight()
{
	//�ŐV�̃��O��2�̎�������Ă���
	return (mouseLog[0] == 2);
}

bool Mouse::IsTriggerLeft()
{
	bool isNow = (mouseLog[0] == 1);//���݂̏��
	bool isLast = (mouseLog[1]);//1�t���[���O�̏��
	return (isNow && !isLast);//�������Ă��āA1�t���[���O������Ă��Ȃ������Ƃ�������
}

bool Mouse::IsTriggerRight()
{
	bool isNow = (mouseLog[0] == 2);//���݂̏��
	bool isLast = (mouseLog[1]);//1�t���[���O�̏��
	return (isNow && !isLast);//�������Ă��āA1�t���[���O������Ă��Ȃ������Ƃ�������
}

bool Mouse::IsRelaseLeft()
{
	bool isNow = (mouseLog[0] == 1);//���݂̏��
	bool isLast = (mouseLog[1]);//1�t���[���O�̏��
	return (!isNow && isLast);//�������Ă��āA1�t���[���O������Ă����Ƃ�����������ɂȂ�
}

bool Mouse::IsRelaseRight()
{
	bool isNow = (mouseLog[0] == 2);//���݂̏��
	bool isLast = (mouseLog[1]);//1�t���[���O�̏��
	return (!isNow && isLast);//�������Ă��āA1�t���[���O������Ă����Ƃ�����������ɂȂ�
}

//�����蔻��
Rect Mouse::GetRect() const
{
	return m_rect;
}

//�J�[�\���̈ʒu�����j���[�ʒu�ɂ��邩�ǂ���
bool Mouse::MouseSelect(int startX, int endX, int startY, int endY)
{
	
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//�}�E�X�̈ʒu���͈͓��ɂ���
	return true;
}

bool Mouse::MouseSelect(float startX, float endX, float startY, float endY)
{
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//�}�E�X�̈ʒu���͈͓��ɂ���
	return true;
}

//�g��쐬
void Mouse::ClickAnimCreate()
{
	Ripples ripples;
	ripples.pos = m_rect.center;//���݂̈ʒu
	ripples.isDisplay = true;//�g���\������
	ripples.idx = 0;//��ԍŏ��̊G�̃C���f�b�N�X

	//��Ԍ��ɓ����
	m_ripples.push_back(ripples);
}

//�g��̍X�V
void Mouse::RipplesAnimUpdate()
{
	for (auto& ripples : m_ripples)
	{
		//���ɕ\���ł��Ȃ��Ȃ��Ă��鎞�͏������Ȃ�
		if (!ripples.isDisplay)	continue;

		//�C���f�b�N�X���A�j���[�V���������ȏ�̂Ƃ�
		if (ripples.idx++ >= kClickAnimNum * kClickAnimSpeed)
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

