#include "FruitsStrawberry.h"
#include "../../Util/DrawFunctions.h"

#include <DxLib.h>
#include "../../game.h"
#include "../HpBar.h"

namespace
{
	constexpr int kSizeW = 32;//�T�C�YX
	constexpr int kSizeH = 32;//�T�C�YX

	constexpr float kScale = 5.0f;//�g�嗦

	constexpr float kSpeed = 1.0f;//�X�s�[�h

	constexpr int kStrawberryMaxHp = 2;//�t���[�c�̍ő�HP

	constexpr float kJumpPower = -16.0f;
}

FruitsStrawberry::FruitsStrawberry(Position2 pos, int handle) : FruitsBase(pos, handle)
{
	m_rect.size = { kSizeW + 20,kSizeH + 20 };
	m_vec.y = kSpeed;
	m_hp->MaxHp(kStrawberryMaxHp);
	m_scale = kScale;
	
	m_jump = kJumpPower;
	m_speed = kSpeed;
}

FruitsStrawberry::~FruitsStrawberry()
{
	
}

void FruitsStrawberry::Draw()
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

std::shared_ptr<FruitsBase> FruitsStrawberry::Clone()
{
	return std::make_shared<FruitsStrawberry>(*this);
}