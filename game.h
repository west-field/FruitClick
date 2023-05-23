#pragma once

namespace Game
{
#ifdef _DEBUG
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = true;
#endif
	//�E�B���h�E��
	const wchar_t* const kTitleText = L"�t���[�c�N���b�N";
	//�E�B���h�E�T�C�Y
	constexpr int kScreenWidth = 640;//160*4;//1600;//16:
	constexpr int kScreenHeight = 480; //90*4;//900;//9:
	//�J���[���[�h
	constexpr int kColorDepth = 32;	//32 or 16

};