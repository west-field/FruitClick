#pragma once

namespace Game
{
#ifdef _DEBUG
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = true;
#endif
	//�E�B���h�E��
	const wchar_t* const kTitleText = L"FruitClick";
	//�E�B���h�E�T�C�Y
	constexpr int kScreenWidth = 640;//16:
	constexpr int kScreenHeight = 480;//9
	//�J���[���[�h
	constexpr int kColorDepth = 32;	//32 or 16

};