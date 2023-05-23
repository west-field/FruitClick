#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"
#include "../InputState.h"

#include "../Util/Sound.h"
#include "../Util/Info.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Mouse.h"

#include "SceneManager.h"
#include "CharacterSelectScene.h"
#include "MonologueScene.h"
#include "SettingScene.h"

namespace
{
	constexpr float kGearScale = 2.0f;//表示拡大率

	constexpr int kTextNumX = 10;
	constexpr int kTextNumY = 5 - 1;

	constexpr int kTextSizeW = 8;
	constexpr int kTextSizeH = 10;
}


TitleScene::TitleScene(SceneManager& manager) : Scene(manager),
	m_updateFunc(&TitleScene::FadeInUpdat), m_selectNum(0), m_moveTitle(0),
	m_moveAdd(1), m_settingH(-1),m_scroll(0)
{
	//BGM
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/title.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	//設定グラフィック
	m_settingH = my::MyLoadGraph(L"Data/Buttons/Settings.png");
	int X = 0, Y = 0;
	GetGraphSize(m_settingH, &X, &Y);
	X = static_cast<int>(X * kGearScale);
	Y = static_cast<int>(Y * kGearScale);
	m_settingRect = { {static_cast<float>(Game::kScreenWidth - X / 2),static_cast<float>(Y/2)}, {X,Y} };
	//背景
	m_bgH = my::MyLoadGraph(L"Data/Background/Gray.png");
}

TitleScene::~TitleScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_settingH);
	DeleteGraph(m_bgH);
}

void
TitleScene::Update(const InputState& input, Mouse& mouse)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input,mouse);

	if (m_scroll++ >= static_cast<int>(kBgSize ))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}
}

void TitleScene::Draw()
{
	//背景
	for (int x = -kBgSize/2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize/2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x+ m_scroll, y+ m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	SetFontSize(kMenuFontSize);
	DrawString( (Game::kScreenWidth - 13 * kMenuFontSize) /2, (Game::kScreenHeight - kMenuFontSize) / 2 + m_moveTitle, L"クリックしてゲームスタート", 0x00fff0);
	SetFontSize(kMenuFontSize+10);
	DrawString( 100, 100, L"Fruits Click", 0x00ff00);
	SetFontSize(0);

	//メニューボックスを描画
	my::MyDrawRectRotaGraph(m_settingRect.center.x, m_settingRect.center.y,
		0, 0, m_settingRect.size.w, m_settingRect.size.h,
		kGearScale, 0.0f, m_settingH, true, false);
#ifdef _DEBUG
	m_settingRect.Draw(0xff00ff);
#endif
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	
	m_moveTitle += m_moveAdd;
	if (m_moveTitle >= kMenuFontSize || m_moveTitle <= 0)
	{
		m_moveAdd *= -1;
	}

	if (mouse.GetRect().IsHit(m_settingRect))
	{
		m_selectNum = static_cast<int>(MenuItem::menuConfig);
	}
	else
	{
		m_selectNum = static_cast<int>(MenuItem::menuGameStart);
	}

	//メニュー
	//「次へ」ボタンが押されたら次シーンへ移行する
	if (input.IsTriggered(InputType::slect))
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

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_S) == 0)
	{
		return;
	}
	else
	{
		m_manager.ChangeScene(new CharacterSelectScene(m_manager));
		return;
	}
#endif
}

void TitleScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new CharacterSelectScene(m_manager));
		return;
	}
}

