#include "FruitsStrawberry.h"
#include "../../Util/DrawFunctions.h"

#include <DxLib.h>
#include "../../game.h"
#include "../HpBar.h"

namespace
{
	constexpr float kSpeed = 1.0f;//スピード

	constexpr int kStrawberryMaxHp = 2;//フルーツの最大HP
	constexpr int kStrawberryPoint = 2;//得点

	constexpr float kJumpPower = -16.0f;
}

FruitsStrawberry::FruitsStrawberry(Position2 pos, int handle) : FruitsBase(pos, handle)
{
	m_rect.size = { kSizeW + 30,kSizeH + 30 };
	m_vec.y = kSpeed;
	m_hp->MaxHp(kStrawberryMaxHp);
	
	m_jump = kJumpPower;
	m_speed = kSpeed;
	m_point = kStrawberryPoint;
}

FruitsStrawberry::~FruitsStrawberry()
{
	
}

void FruitsStrawberry::Draw()
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

std::shared_ptr<FruitsBase> FruitsStrawberry::Clone()
{
	return std::make_shared<FruitsStrawberry>(*this);
}
