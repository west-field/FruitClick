#pragma once
#include "Geometry.h"
//�L�����N�^�[�̎��
enum class CharType
{
	MaskDude,
	NinjaFrog,
	PinkMan,
	VirtualGuy,

	Max
};
//�L�����N�^�[�̃A�j���[�V�������
enum class CharAnimType
{
	Idle,
	Run,
	Jump,
	DoubleJump,
	Fall,

	Max
};

namespace
{
	constexpr int kBgSize = 64;//�w�i�̃T�C�Y
}