#pragma once

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
	DoubleJump,

	Max
};

namespace
{
	constexpr int kBgSize = 64;//背景のサイズ
}