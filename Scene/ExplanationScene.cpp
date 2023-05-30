#include "ExplanationScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/Info.h"
#include "../Util/DrawFunctions.h"

#include "SceneManager.h"
#include "GameplayingScene.h"

namespace
{
	constexpr int kFontSize = 30;//フォントサイズ

	constexpr float kButtonDrawScale = 2.0f;//戻るボタン拡大率

	constexpr float kFruitsDrawScale = 4.0f;//フルーツ拡大率
	constexpr int kFruitsSize= 32;		//フルーツ画像サイズ
	constexpr int kFruitsDrawSize = static_cast<int>(kFruitsSize * kFruitsDrawScale);//フルーツ画面表示サイズ
}

ExplanationScene::ExplanationScene(SceneManager& manager,int selectChar) : Scene(manager),
m_updateFunc(&ExplanationScene::FadeInUpdat), m_drawFunc(&ExplanationScene::RuleExpDraw), m_selectChar(selectChar),
m_frameCount(10)
{
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/charSelect.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);

	m_backH = my::MyLoadGraph(L"Data/UI/Back.png");
	int X = 0, Y = 0;
	GetGraphSize(m_backH, &X, &Y);
	X = static_cast<int>(X * kButtonDrawScale);
	Y = static_cast<int>(Y * kButtonDrawScale);
	m_backRect = { {X / 2.0f,Y / 2.0f},{X,Y} };

	m_bgH = my::MyLoadGraph(L"Data/Background/Blue.png");
	m_scroll = 0;

	/*float addX[3] =
	{
		(640 / 2),
		(640 / 3) / 2,
		(640 / 4) / 2
	};
	float addY = (480 / 3) / 2;
	int handle = my::MyLoadGraph(L"Data/Fruits/Melon.png");
	m_fruitsPos.push_back({ handle, { addX[0], addY }, L"スイカ",0xdc143c, 10 });
	handle = my::MyLoadGraph(L"Data/Fruits/Apple.png");
	m_fruitsPos.push_back({ handle, { addX[1], addY * 3 },L"リンゴ",0xb22222,5 });
	handle = my::MyLoadGraph(L"Data/Fruits/Pineapple.png");
	m_fruitsPos.push_back({ handle, { addX[1] * 5, addY * 3 - 10 }, L"パイナップル",0xff8c00, 8 });
	handle = my::MyLoadGraph(L"Data/Fruits/Bananas.png");
	m_fruitsPos.push_back({ handle, { addX[1] * 3, addY * 3 }, L"バナナ",0xffd700, 5 });
	handle = my::MyLoadGraph(L"Data/Fruits/Kiwi.png");
	m_fruitsPos.push_back({ handle, { addX[2], addY * 5 }, L"キウイ",0x3cb371, 3 });
	handle = my::MyLoadGraph(L"Data/Fruits/Strawberry.png");
	m_fruitsPos.push_back({ handle, { addX[2] * 3, addY * 5 }, L"イチゴ",0xff69b4, 2 });
	handle = my::MyLoadGraph(L"Data/Fruits/Orange.png");
	m_fruitsPos.push_back({ handle, { addX[2] * 5, addY * 5 }, L"オレンジ",0xffa500, 3 });
	handle = my::MyLoadGraph(L"Data/Fruits/Cherries.png");
	m_fruitsPos.push_back({ handle, { addX[2] * 7, addY * 5 }, L"チェリー",0xa52a2a, 2 });*/


	m_hitExpH[0] = my::MyLoadGraph(L"Data/atc1.jpg");
	m_hitExpH[1] = my::MyLoadGraph(L"Data/click1.jpg");
	m_hitExpH[2] = my::MyLoadGraph(L"Data/click2.jpg");
	m_pointExpH = my::MyLoadGraph(L"Data/point.png");//rule3
	m_endExpH[0] = my::MyLoadGraph(L"Data/heat1.jpg");
	m_endExpH[1] = my::MyLoadGraph(L"Data/heat2.jpg");
	m_endExpH[2] = my::MyLoadGraph(L"Data/heat3.jpg");
	m_closeExpH = my::MyLoadGraph(L"Data/rule1.png");

	SetFontSize(kFontSize);//フォントサイズ変更
}

ExplanationScene::~ExplanationScene()
{
	DeleteSoundMem(m_BgmH);

	DeleteGraph(m_backH);
	DeleteGraph(m_bgH);
	/*for (auto& fruit : m_fruitsPos)
	{
		DeleteGraph(fruit.handle);
	}
	m_fruitsPos.clear();*/

	DeleteGraph(m_hitExpH[0]);
	DeleteGraph(m_hitExpH[1]);
	DeleteGraph(m_hitExpH[2]);
	DeleteGraph(m_pointExpH);
	DeleteGraph(m_endExpH[0]);
	DeleteGraph(m_endExpH[1]);
	DeleteGraph(m_endExpH[2]);
	DeleteGraph(m_closeExpH);
}

void
ExplanationScene::Update(const InputState& input,  Mouse& mouse)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input,mouse);

	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}
}

void ExplanationScene::Draw()
{
	//背景
	for (int x = -kBgSize / 2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize / 2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x + m_scroll, y , 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	//戻るボタンを表示
	my::MyDrawRectRotaGraph(m_backRect.center.x, m_backRect.center.y,
		0, 0, m_backRect.size.w, m_backRect.size.h, kButtonDrawScale, 0.0f, m_backH, true, false);

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ExplanationScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &ExplanationScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void ExplanationScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	if (m_frameCount-- <= 0)
	{
		m_frameCount = 0;
	}
	
	if (mouse.GetRect().IsHit(m_backRect) && mouse.IsTriggerLeft())//戻るボタンを押したら一つ前の表示に戻る
	{
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::RuleExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::EndExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::CloseDraw)
		{
			m_drawFunc = &ExplanationScene::EndExpDraw;
			return;
		}
	}
	else if (m_frameCount == 0 && mouse.IsTriggerLeft())//カウントが0の時次の表示に進
	{
		m_frameCount = 10;
		if (m_drawFunc == &ExplanationScene::RuleExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::EndExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::EndExpDraw)
		{
			m_drawFunc = &ExplanationScene::CloseDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::CloseDraw)//最後の表示
		{
			m_updateFunc = &ExplanationScene::FadeOutUpdat;//次のシーンに移行する
			return;
		}
	}
	
}

void ExplanationScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		SetFontSize(0);
		m_manager.ChangeScene(new GameplayingScene(m_manager, m_selectChar));
		return;
	}
}

//ルール説明
void ExplanationScene::RuleExpDraw()
{
	//箱から落ちてくるフルーツをクリックして消して、キャラクタに当たらないようにしよう
	DrawString(Game::kScreenWidth / 4 + 75, 100, L"箱から落ちてくるフルーツを\nクリックして消して", 0x000000);
	DrawString(90, Game::kScreenHeight - 110 , L"キャラクターに\n当たらないようにしよう", 0x000000);
	
	int X = Game::kScreenWidth / 4;//初期位置X
	int Y = Game::kScreenHeight / 4;//初期位置Y

	my::MyDrawRectRotaGraph(X, Y, 0, 0, 115, 122, 1.0f, 0.0f, m_hitExpH[1], true, false);//クリック1
	Y += 122 / 2;
	DrawString(X, Y, L"↓", 0x000000);
	Y += 122 / 2 + kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 0, 0, 115, 122, 1.0f, 0.0f, m_hitExpH[2], true, false);//クリック2
	X *= 3;
	Y += kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 0, 0, 115, 211, 1.0f, 0.0f, m_hitExpH[0], true, false);//キャラクタ
}

//ポイント説明
void ExplanationScene::PointExpDraw()
{
	my::MyDrawRectRotaGraph(642 / 2, 481 / 2, 0, 0, 606, 476, 0.9f, 0.0f, m_pointExpH, true, false);//説明画像を表示
	//フルーツを消すと得点をもらえる。
	DrawString(20, 80, L"フルーツを消すと\n得点をもらえる", 0x000000);
	DrawStringF((640 / 2) + (static_cast<int>(kFruitsDrawSize / 2)) + 30, 80.0f, L"得点が高いほど\nHPも多い", 0x000000);

	//for (auto& fruit : m_fruitsPos)
	//{
	//	my::MyDrawRectRotaGraph(fruit.pos.x, fruit.pos.y, 0, 0, kFruitsSize, kFruitsSize, kFruitsDrawScale, 0.0f, fruit.handle, true, false);//フルーツ画像
	//	DrawStringF(fruit.pos.x - 25, fruit.pos.y - kFruitsSize * 2, fruit.name, fruit.color);//フルーツ名
	//	SetFontSize(kFontSize);//フォントサイズを変更
	//	DrawFormatStringF(fruit.pos.x - kFontSize / 2, fruit.pos.y + kFruitsSize, fruit.color, L"%d点", fruit.point);//得点を表示
	//	SetFontSize(0);//フォントサイズを戻す
	//}
}

//終わりの説明
void ExplanationScene::EndExpDraw()
{
	//キャラクタのハートがすべてなくなると終了
	DrawString(70, 50, L"キャラクタのハートが\n　　　　すべてなくなると終了", 0x000000);
	int X = 180;
	int Y = Game::kScreenHeight / 3;

	my::MyDrawRectRotaGraph(X, Y, 10, 10, 339, 77, 1.0f, 0.0f, m_endExpH[0], true, false);
	Y += 77/2;
	DrawString(X, Y, L"↓", 0x000000);
	X += 339 / 3;
	Y += 77 / 2 + kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 10, 10, 339, 77, 1.0f, 0.0f, m_endExpH[1], true, false);
	Y += 77/2;
	DrawString(X, Y, L"↓", 0x000000);
	X += 339 / 3;
	Y += 77 / 2 + kFontSize;
	my::MyDrawRectRotaGraph(X, Y, 10, 10, 339, 77, 1.0f, 0.0f, m_endExpH[2], true, false);
}

//締め
void ExplanationScene::CloseDraw()
{
	my::MyDrawRectRotaGraph(642 / 2, 481 / 2 + kFontSize, 1, 0, 640, 481, 0.7f, 0.0f, m_closeExpH, true, false);
	//キャラクタを守りながら、得点を集めよう
	DrawString(40, 40, L"キャラクタを守りながら、得点を稼ごう!!", 0x000000);
}
