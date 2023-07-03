#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/Info.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Mouse.h"

#include "SceneManager.h"
#include "CharacterSelectScene.h"
#include "SettingScene.h"

namespace
{
	constexpr int kMenuFontSize = 50;//文字のサイズ
	constexpr float kSettingDrawScale = 2.0f;//設定画像表示拡大率

	constexpr int kTextNumX = 10;
	constexpr int kTextNumY = 5 - 1;

	constexpr int kTextSizeW = 8;
	constexpr int kTextSizeH = 10;
}


TitleScene::TitleScene(SceneManager& manager) : Scene(manager),
	m_updateFunc(&TitleScene::FadeInUpdat), m_selectNum(0), m_instrString(0),
	m_moveAdd(1), m_settingH(-1),m_scroll(0)
{
	//BGM
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/title.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	//設定グラフィック
	m_settingH = my::MyLoadGraph(L"Data/UI/Settings.png");
	int X = 0, Y = 0;
	GetGraphSize(m_settingH, &X, &Y);//サイズ取得
	X = static_cast<int>(X * kSettingDrawScale);//画像を拡大
	Y = static_cast<int>(Y * kSettingDrawScale);
	m_settingRect = { {static_cast<float>(Game::kScreenWidth - X / 2),static_cast<float>(Y/2)}, {X,Y} };//設定画像矩形
	//背景
	m_bgH = my::MyLoadGraph(L"Data/Background/Yellow.png");

	//キャラクター
	m_charPos = {0.0f,static_cast<float>(Game::kScreenHeight - 100) };//位置
	m_char[0] = my::MyLoadGraph(L"Data/Characters/MaskDude/Run.png");//キャラクター
	m_char[1] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Run.png");
	m_char[2] = my::MyLoadGraph(L"Data/Characters/PinkMan/Run.png");
	m_char[3] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Run.png");
	m_charType = 0;//キャラクタータイプ
	m_frameCount = 0;//画像変更
	m_idx = 0;//アニメーション
}

TitleScene::~TitleScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_settingH);
	DeleteGraph(m_bgH);
	for (auto& chartype : m_char)
	{
		DeleteGraph(chartype);
	}
}

void
TitleScene::Update(Mouse& mouse)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(mouse);

	if (m_scroll++ >= static_cast<int>(kBgSize ))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}

	m_charPos.x += 2.0f;//キャラクター移動
	//キャラクターが画面右端まで言ったら
	if (m_charPos.x >= Game::kScreenWidth + 32 / 2)
	{
		m_charPos.x = -32 / 2;//左端へ戻る
		m_charType = GetRand(3);//次に表示するキャラクタータイプをランダムに決める
	}

	//キャラクターアニメーション
	if (m_frameCount-- <= 0)
	{
		if (m_idx++ >= 12 - 1)
		{
			m_idx -= 12;
		}
		m_frameCount = 5;
	}
}

void TitleScene::Draw()
{
	//背景
	for (int x = -kBgSize/2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize/2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x, y+ m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}
	//キャラクター
	for (int i = 0; i < 4; i++)
	{
		if (i != m_charType)	continue;
		my::MyDrawRectRotaGraph(m_charPos.x, m_charPos.y, m_idx * 32, 0, 32, 32, 2.0f, 0.0f, m_char[m_charType], true, false);
		break;
	}
	//文字を表示
	SetFontSize(kMenuFontSize);
	DrawString( (Game::kScreenWidth - 13 * kMenuFontSize) /2, (Game::kScreenHeight - kMenuFontSize) / 2 + m_instrString, L"クリックしてゲームスタート", 0x00fff0);
	SetFontSize(kMenuFontSize+10);
	DrawString( 100, 100, L"Fruits Click", 0x00ff00);
	SetFontSize(0);

	//メニューボックスを描画
	my::MyDrawRectRotaGraph(m_settingRect.center.x, m_settingRect.center.y,
		0, 0, m_settingRect.size.w, m_settingRect.size.h,
		kSettingDrawScale, 0.0f, m_settingH, true, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::FadeInUpdat(Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);//画面のフェードイン
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);//音量のフェードイン
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat( Mouse& mouse)
{
	//次にする指示文字列を縦に揺らす
	m_instrString += m_moveAdd;
	if (m_instrString >= 60 || m_instrString <= 0)
	{
		m_moveAdd *= -1;
	}
	//マウスカーソルが設定矩形と重なっていた時
	if (mouse.GetRect().IsHit(m_settingRect))
	{
		m_selectNum = static_cast<int>(MenuItem::menuConfig);//設定を選択
	}
	else
	{
		m_selectNum = static_cast<int>(MenuItem::menuGameStart);//ゲームスタートを選択
	}

	//マウスが押されたら次シーンへ移行する
	if(mouse.IsTriggerLeft())
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		if (m_selectNum == static_cast<int>(MenuItem::menuConfig))
		{
			m_manager.PushScene(new SettingScene(m_manager,m_BgmH));
			return;
		}
		else if(m_selectNum == static_cast<int>(MenuItem::menuGameStart))
		{
			m_updateFunc = &TitleScene::FadeOutUpdat;
		}
	}
	//右クリックしたとき設定画面を表示する
	if (mouse.IsTriggerRight())
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_manager.PushScene(new SettingScene(m_manager, m_BgmH));
		return;
	}
}

void TitleScene::FadeOutUpdat( Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new CharacterSelectScene(m_manager));
		return;
	}
}

