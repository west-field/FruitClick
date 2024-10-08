#pragma once

namespace Game
{
#ifdef _DEBUG
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = true;
#endif
	//ウィンドウ名
	const wchar_t* const kTitleText = L"FruitClick";
	//ウィンドウサイズ
	constexpr int kScreenWidth = 640;//16:
	constexpr int kScreenHeight = 480;//9
	//カラーモード
	constexpr int kColorDepth = 32;	//32 or 16

};