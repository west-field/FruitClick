#include "Character.h"
#include <DxLib.h>
#include <string>

#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"
#include "../game.h"
namespace
{
	constexpr int kCharSizeW = 32;//サイズX
	constexpr int kCharSizeH = 32;//サイズY

	constexpr int kCharAnimNum = 11;//画像枚数
	constexpr int kCharAnimSpeed = 5;//画像移動スピード

	constexpr float kCharSrawScale = 2.0f;//表示拡大率

	constexpr int kUltimateFrame = 120;//無敵時間 2秒

	constexpr float kCharMoveSpeed = 2.0f;//キャラクタ移動スピード

	constexpr int kMaxHp = 10;//キャラクターの最大HP
}

Character::Character(int selectChar,Position2 pos):m_idxX(0),m_idxY(0)
{
	const wchar_t* name[static_cast<int>(CharType::Max)] =
	{
		L"MaskDude",
		L"NinjaFrog",
		L"PinkMan",
		L"VirtualGuy",
	};
	CharacterLoad(name[selectChar]);

	m_maxHp = kMaxHp;
	m_moveSpeed = kCharMoveSpeed;
	m_animSpeed = kCharAnimSpeed;
	m_ultimateFrame = kUltimateFrame;

	int hitW = 30;
	int hitH = 35;

	switch (selectChar)
	{
	case 1://HPが多い
		m_maxHp += 5;
		break;
	case 2://スピードが早い
		m_animSpeed -= 2;
		m_moveSpeed += 0.5f;
		break;
	case 3://当たり判定が小さい
		hitW += 5;
		hitH += 5;
		break;
	case 4://無敵時間が長い
		m_ultimateFrame += 10;
		break;
	default:
		break;
	}

	int  sizeW = static_cast<int>(kCharSizeW * kCharSrawScale);
	int  sizeH = static_cast<int>(kCharSizeH * kCharSrawScale);

	m_rect = { pos,{ sizeW - hitW,sizeH - hitH } };
	
	m_animType = CharAnimType::Run;

	m_hp = std::make_shared<HpBar>(Position2{0.0f,0.0f});
	m_hp->MaxHp(m_maxHp);

	m_vecX = m_moveSpeed;
}

Character::~Character()
{
	for (auto& handle : m_handle)
	{
		DeleteGraph(handle);
	}
}

void Character::Update()
{
	m_rect.center.x += m_vecX;

	m_frame--;

	if (m_frame <= 0)
	{
		int type = static_cast<int>(m_animType);
		if (m_idxX++ >= m_w[type]-1)
		{
			m_idxX -= m_w[type];
		}
		m_frame = m_animSpeed;
	}

	//0よりも小さかったら0を返す
	if (--m_ultimateTimer <= 0)
	{
		m_ultimateTimer = 0;
	}

	float w = static_cast<float>(kCharSizeW / 2);
	if (m_rect.center.x <= 0 + w)
	{
		m_rect.center.x = w;
		m_isLeft = !m_isLeft;
		m_vecX *= -1.0f;
	}
	if (m_rect.center.x >= Game::kScreenWidth - w)
	{
		m_rect.center.x = Game::kScreenWidth - w;
		m_isLeft = !m_isLeft;
		m_vecX *= -1.0f;
	}
}

void Character::Update(bool isPlay)
{
	if (m_rect.center.x <= Game::kScreenWidth / 2 - m_moveSpeed)
	{
		m_rect.center.x += m_moveSpeed;
		if (m_isLeft != false)
		{
			m_isLeft = !m_isLeft;
		}
	}
	else if (m_rect.center.x >= Game::kScreenWidth / 2 + m_moveSpeed)
	{
		m_rect.center.x -= m_moveSpeed;
		if (m_isLeft != true)
		{
			m_isLeft = !m_isLeft;
		}
	}
	else if(!isPlay)
	{
		if (m_animType != CharAnimType::DoubleJump)
		{
			m_animType = CharAnimType::DoubleJump;
			m_idxX = 0;
		}
	}
	else if (isPlay)
	{
		if (m_animType != CharAnimType::Run)
		{
			m_animType = CharAnimType::Run;
			m_idxX = 0;
		}
	}

	m_frame--;
	if (m_frame <= 0)
	{
		int type = static_cast<int>(m_animType);
		if (m_idxX++ >= m_w[type] - 1)
		{
			if (!isPlay)
			{
				if (type == static_cast<int>(CharAnimType::DoubleJump))
				{
					m_idxX = m_w[type] - 1;
				}
				else
				{
					m_idxX -= m_w[type];
				}
			}
			else
			{
				m_idxX -= m_w[type];
			}
		}
		m_frame = m_animSpeed;
	}
	//0よりも小さかったら0を返す
	if (m_ultimateTimer != 0)
	{
		m_ultimateTimer = 0;
	}
}

void Character::Draw()
{
	if ((m_ultimateTimer / 10) % 2 == 1)
	{
		return;
	}
	my::MyDrawRectRotaGraph(
		m_rect.center.x, m_rect.center.y,
		m_idxX * kCharSizeW, m_idxY * kCharSizeH,
		kCharSizeW, kCharSizeH,
		kCharSrawScale, 0.0f, m_handle[static_cast<int>(m_animType)], true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
	int type = static_cast<int>(m_animType);
	DrawFormatString(0, 60, 0xffffff, L"%d,%d", m_w[type],m_idxX);
#endif
}

void Character::CharacterLoad(const wchar_t* charName)
{
	std::wstring file = L"Data/Characters/";
	file += charName;

	const wchar_t* name[static_cast<int>(CharAnimType::Max)] =
	{
		L"/Idle.png",
		L"/Run.png",
		L"/Jump.png",
		L"/Fall.png",
		L"/DoubleJump.png",
	};

	std::wstring path;
	for (int i = 0; i < static_cast<int>(CharAnimType::Max); i++)
	{
		path = file;
		path += name[i];

		//それぞれのアニメーションをロードする
		m_handle[i] = my::MyLoadGraph(path.c_str());

		int h = 0;
		//グラフィックのサイズを取得
		GetGraphSize(m_handle[i], &m_w[i], &h);
		//横に何個並んでいるか
		m_w[i] = m_w[i] / kCharSizeW;
	}
}

const Rect& Character::GetRect() const
{
	return m_rect;
}

bool Character::IsCollidable() const
{
	return m_ultimateTimer == 0;
}

void Character::Damage(int damage)
{
	m_ultimateTimer = m_ultimateFrame;
	m_hp->Damage(damage);
}

void Character::Heal(int Heal)
{
	m_hp->Heal(Heal);
}

int Character::GetHp() const
{
	return m_hp->GetHp();
}

CharAnimType Character::GetAnimType() const
{
	return m_animType;
}

void Character::SetAnimType(CharAnimType type)
{
	m_animType = type;
}

int Character::GetIdx() const
{
	if (m_animType != CharAnimType::DoubleJump)
	{
		return -1;
	}
	return m_idxX;
}

int Character::GetAnimNum(CharAnimType type) const
{
	return  m_w[static_cast<int>(type)] - 1;
}
