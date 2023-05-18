#include "FruitsApple.h"
#include "../../Util/DrawFunctions.h"

#include <DxLib.h>
#include "../../game.h"
#include "../HpBar.h"

namespace
{
	constexpr float kSpeed = 1.0f;//スピード

	constexpr int kAppleMaxHp = 5;//フルーツの最大HP
	constexpr int kApplePoint = 5;//得点

	constexpr float kJumpPower = -16.0f;
}

FruitsApple::FruitsApple(Position2 pos, int handle) : FruitsBase(pos, handle)
{
	m_rect.size = { kSizeW + 20,kSizeH + 20 };
	m_hp->MaxHp(kAppleMaxHp);
	
	m_jump = kJumpPower;
	m_speed = kSpeed;
	m_point = kApplePoint;
}

FruitsApple::~FruitsApple()
{
	
}

void FruitsApple::Draw()
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

std::shared_ptr<FruitsBase> FruitsApple::Clone()
{
	return std::make_shared<FruitsApple>(*this);
}
