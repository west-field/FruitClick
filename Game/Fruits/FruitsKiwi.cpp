#include "FruitsKiwi.h"
#include "../../Util/DrawFunctions.h"

#include <DxLib.h>
#include "../../game.h"
#include "../HpBar.h"

namespace
{
	constexpr float kSpeed = 1.0f;//�X�s�[�h

	constexpr int kKiwiMaxHp = 3;//�t���[�c�̍ő�HP
	constexpr int kKiwiPoint = 3;//���_

	constexpr float kJumpPower = -16.0f;
}

FruitsKiwi::FruitsKiwi(Position2 pos, int handle) : FruitsBase(pos, handle)
{
	m_rect.size = { kSizeW + 30,kSizeH + 30 };
	m_vec.y = kSpeed;
	m_hp->MaxHp(kKiwiMaxHp);
	
	m_jump = kJumpPower;
	m_speed = kSpeed;
	m_point = kKiwiPoint;
}

FruitsKiwi::~FruitsKiwi()
{
	
}

void FruitsKiwi::Draw()
{
	int img = m_idxX / kAnimSpeed;
	if (m_blend != 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, m_blend);
	}
	my::MyDrawRectRotaGraph(m_rect.center.x, m_rect.center.y, img * kSizeW, 0, kSizeW, kSizeH, m_scale, 0.0f, m_handle, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
	m_rect.Draw(0x0000ff);
	DrawFormatString(10, 0, 0xffffff, L"x%f,y%f", m_rect.center.x, m_rect.center.y);
#endif
}

std::shared_ptr<FruitsBase> FruitsKiwi::Clone()
{
	return std::make_shared<FruitsKiwi>(*this);
}
