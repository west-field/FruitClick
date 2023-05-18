#include "FruitsBase.h"
#include "../HpBar.h"
#include <DxLib.h>
#include "../../game.h"

namespace
{
	constexpr float kScale = 5.0f;//�g�嗦

	constexpr float kGravity = 2.0f;//�d��
	constexpr int kFlashingTime = 5;//�_�Ŏ���
}

FruitsBase::FruitsBase(Position2 pos, int handle):
	m_handle(-1), m_blend(0),
	m_scale(0.0f), m_rect(), m_vec(),
	m_isExist(true),m_flashingTimer(0),m_idxX(0),
	m_jump(0),m_speed(0), m_isDestroy(false), m_point(0)
{
	m_handle = handle;
	m_scale = kScale;
	m_rect.center = pos;
	m_hp = std::make_shared<HpBar>(Position2{});
}

FruitsBase::~FruitsBase()
{
}

void FruitsBase::Update()
{
	m_jump += kGravity;
	//��ɏオ��
	if (m_jump >= m_speed * 2)
	{
		m_jump = m_speed * 2;
	}
	m_rect.center.y += m_jump;

	if (m_flashingTimer-- <= 0)//�U�����󂯂��Ƃ�
	{
		m_blend = 0;
		m_flashingTimer = 0;
	}
	//�͈͊O�ɏo�������
	if (m_rect.center.y >= Game::kScreenHeight + m_rect.size.w)
	{
		m_isExist = false;
	}

	m_idxX = (m_idxX + 1) % (kAnimNum * kAnimSpeed);
}

const Rect& FruitsBase::GetRect() const
{
	return m_rect;
}

void FruitsBase::SetPos(const Position2& pos)
{
	m_rect.center = pos;
}

void FruitsBase::OnDamage(int i)
{
	m_hp->Damage(i);
	m_flashingTimer = kFlashingTime;//�_�Ŏ���
	m_blend = 255;
	//HP�������ȉ��ɂȂ�����
	if (m_hp->GetHp() <= m_hp->GetMaxHp() / 2)
	{
		//�t���[�c�̃T�C�Y��ύX
		float num = kScale / m_hp->GetMaxHp();//�X�P�[�����ő�HP�Ŋ���=
		m_scale = m_scale - num;
		//�����蔻����Ή��̃T�C�Y�ɕύX
		int sizeh = static_cast<int>(m_rect.size.h / m_scale);//���̃T�C�Y���g�嗦�Ŋ���
		int sizew = static_cast<int>(m_rect.size.w / m_scale);
		m_rect.size.h -= sizeh;
		m_rect.size.w -= sizew;
	}

	//HP���O�����̎��͑��݂�����
	if (m_hp->GetHp() <= 0)
	{
		m_isExist = false;
	}
}

int FruitsBase::GetHp()
{
	return m_hp->GetHp();
}

bool FruitsBase::IsCollidable() const
{
	return m_flashingTimer == 0;
}
