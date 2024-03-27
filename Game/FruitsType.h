#pragma once

/// <summary>
/// フルーツのタイプ
/// </summary>
enum class FruitsType
{
	Apple,//リンゴ
	Banana,//バナナ
	Cherrie,//サクランボ
	Kiwi,//キウイ
	Melon,//スイカ
	Orange,//オレンジ
	Pineapple,//パイナップル
	Strawberry,//イチゴ
};

/// <summary>
/// 挙動の種類
/// </summary>
enum class FruitsSpawnId
{
	AppleStraight,		//まっすぐ落ちるリンゴ
	BananaStraight,		//まっすぐ落ちるバナナ
	CherrieStraight,	//まっすぐ落ちるサクランボ
	KiwiStraight,		//まっすぐ落ちるキウイ
	MelonStraight,		//まっすぐ落ちるスイカ
	OrangeStraight,		//まっすぐ落ちるオレンジ
	PineappleStraight,	//まっすぐ落ちるパイナップル
	StrawberryStraight,	//まっすぐ落ちるイチゴ

	Max
};
