#include "FruitsPineapple.h"
#include "../../Util/DrawFunctions.h"

#include <DxLib.h>
#include "../../game.h"
#include "../HpBar.h"

namespace
{
	constexpr float kSpeed = 1.0f;//スピード

	constexpr int kPineappleMaxHp = 4;//フルーツの最大HP
	constexpr int kPineapplePoint = 8;//得点

	constexpr float kUpperPower = -16.0f;//上方向への力
}

FruitsPineapple::FruitsPineapple(Position2 pos, int handle) : FruitsBase(pos, handle)
{
	m_rect.size = { kSizeW + 30,kSizeH + 50 };
	m_vec.y = kSpeed;
	m_hp->MaxHp(kPineappleMaxHp);
	
	m_upperPower = kUpperPower;
	m_speed = kSpeed;
	m_point = kPineapplePoint;
}

FruitsPineapple::~FruitsPineapple()
{
	
}

void FruitsPineapple::Draw()
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
#endif
}

std::shared_ptr<FruitsBase> FruitsPineapple::Clone()
{
	return std::make_shared<FruitsPineapple>(*this);
}
