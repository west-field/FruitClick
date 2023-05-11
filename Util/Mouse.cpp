#include "Mouse.h"
#include <DxLib.h>
#include "DrawFunctions.h"
#include "../InputState.h"
#include "../game.h"

namespace
{
	constexpr int kClickImgWidth = 100;//�摜�T�C�YX
	constexpr int kClickImgHeight = 100;//�摜�T�C�YY
	constexpr float kClickDrawScale = 1.5f;//�g�嗦
	constexpr int kClickAnimNum = 31;//�A�j���[�V��������
	constexpr float  kClickAnimSpeed = 0.5f;//�A�j���[�V�����X�s�[�h
}

Mouse::Mouse():mouseLog()
{
	m_rect = { {},{} };
	m_mouseH = my::MyLoadGraph(L"Data/Cursor.png");
	MouseReset();
}

Mouse::~Mouse()
{
	DeleteGraph(m_mouseH);
}

void Mouse::Update(const InputState& input)
{
	//���O�̍X�V
	for (int i = kLogNum - 1; i >= 1; i--)
	{
		mouseLog[i] = mouseLog[i - 1];
	}

	//�ŐV�̏��
	if (input.IsTriggered(InputType::slect)|| input.IsTriggered(InputType::prev))
	{
		mouseLog[0] = 1;
	}
	else
	{
		mouseLog[0] = 0;
	}

	m_rect.center = GetPos();
}

void Mouse::Draw()
{
	my::MyDrawGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), m_mouseH, true);
#ifdef _DEBUG
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0xffffff, L"-----x%3f,y%3f", m_rect.center.x, m_rect.center.y);
#endif
}

void Mouse::MouseReset()
{
	SetMousePoint(Game::kScreenWidth / 2, Game::kScreenHeight / 2);
}

Position2 Mouse::GetPos()
{
	Position2 mousePos{ 0,0 };
	int mouseX = 0;
	int mouseY = 0;
	if (GetMousePoint(&mouseX, &mouseY) == -1)
	{
		//�G���[����
		return mousePos;
	}
	mousePos.x = static_cast<float>(mouseX);
	mousePos.y = static_cast<float>(mouseY);

	return mousePos;
}

bool Mouse::IsPressLeft()
{
	//�ŐV�̃��O��1�̎�������Ă���
	return (mouseLog[0]);
}

bool Mouse::IsTriggerLeft()
{
	bool isNow = (mouseLog[0]);//���݂̏��
	bool isLast = (mouseLog[1]);//1�t���[���O�̏��
	return (isNow && !isLast);//�������Ă��āA1�t���[���O������Ă��Ȃ������Ƃ�������
}

bool Mouse::IsRelaseLeft()
{
	bool isNow = (mouseLog[0]);//���݂̏��
	bool isLast = (mouseLog[1]);//1�t���[���O�̏��
	return (!isNow && isLast);//�������Ă��āA1�t���[���O������Ă����Ƃ�����������ɂȂ�
}

Rect& Mouse::GetRect()
{
	return m_rect;
}

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

