#include "FruitsBase.h"
#include "../HpBar.h"
#include <DxLib.h>
#include "../../game.h"

namespace
{
	constexpr float kScale = 5.0f;//拡大率

	constexpr float kGravity = 2.0f;//重力
	constexpr int kFlashingTime = 5;//点滅時間
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
	//上に上がる
	if (m_jump >= m_speed * 2)
	{
		m_jump = m_speed * 2;
	}
	m_rect.center.y += m_jump;

	if (m_flashingTimer-- <= 0)//攻撃を受けたとき
	{
		m_blend = 0;
		m_flashingTimer = 0;
	}
	//範囲外に出たら消す
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
	m_flashingTimer = kFlashingTime;//点滅時間
	m_blend = 255;
	//HPが半分以下になったら
	if (m_hp->GetHp() <= m_hp->GetMaxHp() / 2)
	{
		//フルーツのサイズを変更
		float num = kScale / m_hp->GetMaxHp();//スケールを最大HPで割る=
		m_scale = m_scale - num;
		//当たり判定も対応のサイズに変更
		int sizeh = static_cast<int>(m_rect.size.h / m_scale);//今のサイズを拡大率で割る
		int sizew = static_cast<int>(m_rect.size.w / m_scale);
		m_rect.size.h -= sizeh;
		m_rect.size.w -= sizew;
	}

	//HPが０未満の時は存在を消す
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
