#include "FruitsPineapple.h"
#include "../../Util/DrawFunctions.h"

#include <DxLib.h>
#include "../../game.h"
#include "../HpBar.h"

namespace
{
	constexpr int kSizeW = 32;//サイズX
	constexpr int kSizeH = 32;//サイズX

	constexpr float kScale = 5.0f;//拡大率

	constexpr float kSpeed = 1.0f;//スピード

	constexpr int kPineappleMaxHp = 4;//フルーツの最大HP

	constexpr float kJumpPower = -16.0f;
}

FruitsPineapple::FruitsPineapple(Position2 pos, int handle) : FruitsBase(pos, handle)
{
	m_rect.size = { kSizeW + 20,kSizeH + 40 };
	m_vec.y = kSpeed;
	m_hp->MaxHp(kPineappleMaxHp);
	m_scale = kScale;
	
	m_jump = kJumpPower;
	m_speed = kSpeed;
}

FruitsPineapple::~FruitsPineapple()
{
	
}

void FruitsPineapple::Draw()
{
	if (m_blend != 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, m_blend);
	}
	my::MyDrawRectRotaGraph(m_rect.center.x, m_rect.center.y, m_idxX * kSizeW, m_idxY * kSizeH, kSizeW, kSizeH, m_scale, 0.0f, m_handle, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
	m_rect.Draw(0x0000ff);
	DrawFormatString(10, 0, 0xffffff, L"x%f,y%f", m_rect.center.x, m_rect.center.y);
#endif
}

std::shared_ptr<FruitsBase> FruitsPineapple::Clone()
{
	return std::make_shared<FruitsPineapple>(*this);
}
