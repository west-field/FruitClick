#include "HpBar.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	constexpr float kScale = 0.4f;//拡大率
	constexpr int kHpMax = 10;//最大値指定がなかった時の最大値
}

HpBar::HpBar(Position2 pos):m_MaxHp(0),m_Hp(0),m_HpHandle(-1), m_heatFull(-1), m_heatHalf(-1)
{
	m_rect = { pos ,{} };
}

HpBar::~HpBar()
{
	m_heatHandle.clear();
	DeleteGraph(m_HpHandle);
	DeleteGraph(m_heatFull);
	DeleteGraph(m_heatHalf);
}

void HpBar::Init()
{
	m_HpHandle = my::MyLoadGraph(L"Data/UI/HeartContainer.png");
	m_heatFull = my::MyLoadGraph(L"Data/UI/HeatFull.png");
	m_heatHalf = my::MyLoadGraph(L"Data/UI/HeartHalf.png");

	GetGraphSize(m_HpHandle, &m_rect.size.w, &m_rect.size.h);

	m_MaxHp = m_Hp = kHpMax;
	SetHeat();
}

void HpBar::MaxHp(int maxHp)
{
	m_MaxHp = m_Hp = maxHp;
	SetHeat();
}

void HpBar::SetHeat()
{
	m_heatHandle.clear();
	m_heatHandle.resize(m_MaxHp / 2);

	for (auto& heat : m_heatHandle)
	{
		heat.type = HeatType::Full;
		heat.posAddX =  3;
		heat.posAddY = -2;
		heat.isExist = true;
	}
}

void HpBar::UpdatePlayer()
{
	if (m_heatHandle.back().type == HeatType::Full)
	{
		m_heatHandle.back().type = HeatType::Half;
		m_heatHandle.back().posAddX = -2;
		m_heatHandle.back().posAddY = 0;
	}
	else
	{
		m_heatHandle.back().isExist = false;
	}

	m_heatHandle.remove_if([](Heat heat) {
		return !heat.isExist;
		});
}

void HpBar::Draw()
{
	//中心位置
	int x = static_cast<int>(m_rect.center.x + m_rect.size.w * kScale / 2);
	int y = static_cast<int>(m_rect.center.y - m_rect.size.h * kScale / 2);

	//枠
	for (int i = 0; i < m_MaxHp / 2; i++)
	{
		x += static_cast<int>(m_rect.size.w  * kScale);
		my::MyDrawRectRotaGraph(x, y,
			idxX * m_rect.size.w, idxY * m_rect.size.h,
			m_rect.size.w, m_rect.size.h, kScale, 0.0f, m_HpHandle, true, false);
	}
	//ハートを表示する
	x = static_cast<int>(m_rect.center.x + m_rect.size.w * kScale / 2);
	for (auto& heat : m_heatHandle)
	{
		x += static_cast<int>(m_rect.size.w * kScale);
		if (!heat.isExist)	continue;
		int handle = m_heatFull;
		if (heat.type == HeatType::Half)
		{
			handle = m_heatHalf;
		}

		my::MyDrawRectRotaGraph(x + heat.posAddX, y + heat.posAddY,
			idxX * m_rect.size.w, idxY * m_rect.size.h,
			m_rect.size.w, m_rect.size.h, kScale, 0.0f, handle, true, false);
	}

#ifdef _DEBUG
	DrawFormatString(x, y, 0x000000, L"hp%d", m_Hp);
	DrawFormatString(x, y + 20, 0x000000, L"x%d,y%d", idxX, idxY);
#endif
}

void HpBar::Damage(int damage)
{
	SetHp(m_Hp - damage);
}

void HpBar::Heal(const int heal)
{
	SetHp(m_Hp + heal);
}

void HpBar::SetHp(const int hp)
{
	if (hp >= m_MaxHp)
	{
		m_Hp = m_MaxHp;
	}
	else if (hp < 0)
	{
		m_Hp = 0;
	}
	else
	{
		m_Hp = hp;
	}
}
