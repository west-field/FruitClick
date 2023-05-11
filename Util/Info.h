#pragma once
#include "Geometry.h"
//キャラクターの種類
enum class CharType
{
	MaskDude,
	NinjaFrog,
	PinkMan,
	VirtualGuy,

	Max
};
//キャラクターのアニメーション種類
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
	constexpr int kBgSize = 64;//背景のサイズ
}