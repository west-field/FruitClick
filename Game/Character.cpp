#include "Character.h"
#include <DxLib.h>
#include <string>

#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"
#include "../game.h"
namespace
{
	constexpr int kCharSizeW = 32;//�T�C�YX
	constexpr int kCharSizeH = 32;//�T�C�YY

	constexpr int kCharAnimNum = 11;//�摜����
	constexpr int kCharAnimSpeed = 5;//�摜�ړ��X�s�[�h

	constexpr float kCharSrawScale = 2.0f;//�\���g�嗦

	constexpr int kUltimateFrame = 120;//���G���� 2�b

	constexpr float kCharMoveSpeed = 2.0f;//�L�����N�^�ړ��X�s�[�h

	constexpr int kMaxHp = 10;//�L�����N�^�[�̍ő�HP
}

Character::Character(int selectChar):m_idxX(0),m_idxY(0)
{
	const wchar_t* name[static_cast<int>(CharType::Max)] =
	{
		L"MaskDude",
		L"NinjaFrog",
		L"PinkMan",
		L"VirtualGuy",
	};
	CharacterLoad(name[selectChar]);

	int  sizeW = static_cast<int>(kCharSizeW * kCharSrawScale);
	int  sizeH = static_cast<int>(kCharSizeH * kCharSrawScale);

	m_rect = { {static_cast<float>(Game::kScreenWidth / 2),static_cast<float>(Game::kScreenHeight - 100)},
				{ sizeW - 30,sizeH - 35 } };
	
	m_animType = CharAnimType::Run;

	m_hp = std::make_shared<HpBar>(Position2{0.0f,0.0f});
	m_hp->MaxHp(kMaxHp);

	m_vecX = kCharMoveSpeed;
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

	if (m_frame == 0)
	{
		int type = static_cast<int>(m_animType);
		if (m_idxX++ >= m_w[type]-1)
		{
			m_idxX -= m_w[type];
		}
	}

	if (m_frame <= 0)
	{
		m_frame = kCharAnimSpeed;
	}

	//0����������������0��Ԃ�
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
		L"/DoubleJump.png",
		L"/Fall.png",
	};

	std::wstring path;
	for (int i = 0; i < static_cast<int>(CharAnimType::Max); i++)
	{
		path = file;
		path += name[i];

		//���ꂼ��̃A�j���[�V���������[�h����
		m_handle[i] = my::MyLoadGraph(path.c_str());

		int h = 0;
		//�O���t�B�b�N�̃T�C�Y���擾
		GetGraphSize(m_handle[i], &m_w[i], &h);
		//���ɉ�����ł��邩
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
	m_ultimateTimer = kUltimateFrame;
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