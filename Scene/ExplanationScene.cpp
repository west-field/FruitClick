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

	constexpr float kButtonDrawScale = 2.0f;//拡大率
	//プレイヤーグラフィック
	constexpr int kGraphSizeWidth = 32;		//サイズ
	constexpr int kGraphSizeHeight = 32;	//サイズ
	constexpr float kDrawScale = 2.0f;		//拡大率
	constexpr int kFrameSpeed = 10;		//アニメーションスピード
}

ExplanationScene::ExplanationScene(SceneManager& manager,int selectChar) : Scene(manager),
m_updateFunc(&ExplanationScene::FadeInUpdat), m_drawFunc(&ExplanationScene::NormalDraw), m_selectChar(selectChar),
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

	m_fruitH = my::MyLoadGraph(L"Data/PointExp.png");
	m_bgH = my::MyLoadGraph(L"Data/Background/Blue.png");
	m_scroll = 0;

	float addX[3] =
	{
		(640 / 2),
		(640 / 3) / 2,
		(640 / 4) / 2
	};
	float addY = (480 / 3) / 2;

	m_fruitsPos.push_back({ { addX[0], addY }, L"スイカ",0xdc143c, 10 });
	m_fruitsPos.push_back({ { addX[1], addY * 3 },L"リンゴ",0xb22222,5 });
	m_fruitsPos.push_back({ { addX[1] * 5, addY * 3 - 10 }, L"パイナップル",0xff8c00, 8 });
	m_fruitsPos.push_back({ { addX[1] * 3, addY * 3 }, L"バナナ",0xffd700, 5 });
	m_fruitsPos.push_back({ { addX[2], addY * 5 }, L"キウイ",0x3cb371, 3 });
	m_fruitsPos.push_back({ { addX[2] * 3, addY * 5 }, L"イチゴ",0xff69b4, 2 });
	m_fruitsPos.push_back({ { addX[2] * 5, addY * 5 }, L"オレンジ",0xffa500, 3 });
	m_fruitsPos.push_back({ { addX[2] * 7, addY * 5 }, L"チェリー",0xa52a2a, 2 });
}

ExplanationScene::~ExplanationScene()
{
	DeleteSoundMem(m_BgmH);

	DeleteGraph(m_backH);
	DeleteGraph(m_fruitH);
	DeleteGraph(m_bgH);
	m_fruitsPos.clear();
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

	DrawString(0, 0, L"説明", 0xffffff);

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
		if (m_drawFunc == &ExplanationScene::FirstExpDraw)
		{
			m_drawFunc = &ExplanationScene::NormalDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::FirstExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::ThirdExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::FourthExpDraw)
		{
			m_drawFunc = &ExplanationScene::ThirdExpDraw;
			return;
		}
	}
	else if (m_frameCount == 0 && mouse.IsTriggerLeft())//カウントが0の時次の表示に進
	{
		m_frameCount = 10;
		if (m_drawFunc == &ExplanationScene::NormalDraw)
		{
			m_drawFunc = &ExplanationScene::FirstExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::FirstExpDraw)
		{
			m_drawFunc = &ExplanationScene::PointExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::PointExpDraw)
		{
			m_drawFunc = &ExplanationScene::ThirdExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::ThirdExpDraw)
		{
			m_drawFunc = &ExplanationScene::FourthExpDraw;
			return;
		}
		if (m_drawFunc == &ExplanationScene::FourthExpDraw)//最後の表示
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
		m_manager.ChangeScene(new GameplayingScene(m_manager, m_selectChar));
		return;
	}
}

void ExplanationScene::NormalDraw()
{
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"normal", 0xffffff);
}

void ExplanationScene::FirstExpDraw()
{
	//箱から落ちてくるフルーツをクリックして消して、キャラクタに当たらないようにしよう
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"1", 0xffffff);
	DrawString(100, 100, L"箱から落ちてくるフルーツを", 0xffffff);
	DrawString(100, 120, L"クリックして消して、", 0xffffff);
	DrawString(100, 140, L"キャラクタに当たらないようにしよう", 0xffffff);
}

void ExplanationScene::PointExpDraw()
{
	float scale = 4.0f;
	int size = static_cast<int>(32 * scale);
	int halfSize = static_cast<int>(size / 2);//表示する大きさの半分
	int add = 25;//文字を絵の上に表示できるように
	int sud = 32 * 2;//文字を左にずらす
	int fontsize = kFontSize / 2;

	//フルーツを消すと得点をもらえる。
	DrawString(80, 80, L"フルーツを消すと\n得点をもらえる", 0x000000);
	DrawStringF(m_fruitsPos[0].pos.x + halfSize + 50, 80.0f, L"得点が高いほど\nHPも多い", 0x000000);

	for (auto& fruit : m_fruitsPos)
	{
		int left = static_cast<int>(fruit.pos.x - halfSize);
		int top = static_cast<int>(fruit.pos.y - halfSize);

		my::MyDrawRectRotaGraph(fruit.pos.x, fruit.pos.y, left,top , size, size, 1.0f, 0.0f, m_fruitH, true, false);
		DrawStringF(fruit.pos.x - add, fruit.pos.y - sud, fruit.name, fruit.color);
		SetFontSize(kFontSize);
		DrawFormatStringF(fruit.pos.x - fontsize, fruit.pos.y + 32, fruit.color, L"%d点", fruit.point);
		SetFontSize(0);
	}
}

void ExplanationScene::ThirdExpDraw()
{
	//キャラクタのハートがすべてなくなると終了
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"3", 0xffffff);
	DrawString(100, 100, L"キャラクタのハードがすべてなくなると終了", 0xffffff);
}

void ExplanationScene::FourthExpDraw()
{
	//キャラクタを守りながら、得点を集めよう
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"4", 0xffffff);
	DrawString(100, 100, L"キャラクタを守りながら、得点を集めよう!!", 0xffffff);
}
