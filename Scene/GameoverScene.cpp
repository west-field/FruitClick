#include "GameoverScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameplayingScene.h"

namespace
{
	constexpr float kMoveNum = 2.0f;//プレイヤー移動スピード
	constexpr int kMojiSize = 90;//文字の大きさ
}

GameoverScene::GameoverScene(SceneManager& manager) :
	Scene(manager),  m_updateFunc(&GameoverScene::FadeInUpdat),
	m_drawFunc(&GameoverScene::NormalDraw) 
{
	float posX = (Game::kScreenWidth - kMojiNum * kMojiSize) / 2;
	for (int i = 0; i < kMojiNum; i++)
	{
		m_moji[i].pos = { static_cast<float>(posX + i * kMojiSize) ,Game::kScreenHeight / 3 };
		m_moji[i].moveY = i * -1.0f;
		m_moji[i].add = 0.5f;
	}
	//m_BgmH = LoadSoundMem(L"Sound/BGM/shizukanoumi.mp3");
}

GameoverScene::~GameoverScene()
{
	DeleteSoundMem(m_BgmH);
	SoundManager::GetInstance().StopBgm(SoundId::Gameover);
}

void
GameoverScene::Update(const InputState& input,  Mouse& mouse)
{
	(this->*m_updateFunc)(input,mouse);
}

void
GameoverScene::Draw()
{
	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameoverScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameoverScene::NormalUpdat;
		m_fadeColor = 0x000000;
		m_fadeValue = 0;
		SoundManager::GetInstance().Play(SoundId::Gameover);
	}
}

void GameoverScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	if (input.IsTriggered(InputType::slect))
	{
		m_updateFunc = &GameoverScene::MojiUpdate;
		m_drawFunc = &GameoverScene::MojiDraw;
		ChangeVolumeSoundMem(0, m_BgmH);
		PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
		return;
	}
}

void GameoverScene::MojiUpdate(const InputState& input,  Mouse& mouse)
{
	if (m_soundVolume++ >= SoundManager::GetInstance().GetBGMVolume())
	{
		m_soundVolume = SoundManager::GetInstance().GetBGMVolume();
	}
	ChangeVolumeSoundMem(m_soundVolume, m_BgmH);

	//文字を揺らす
	for (auto& moji : m_moji)
	{
		if (moji.moveY > kMojiNum)
		{
			moji.add *= -1.0f;
		}
		else if (moji.moveY < -kMojiNum)
		{
			moji.add *= -1.0f;
		}
		moji.moveY += moji.add;
	}

	//メニュー
	bool isPress = false;//キーが押されたかどうか
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % menuNum;//選択状態を一つ下げる
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//選択状態を一つ上げる
		isPress = true;
	}

	if (isPress)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].x = kMovedPosX;//移動位置に移動する
				SelectMenu[i].color = 0xffa0aa;//色を変える
			}
			else
			{
				SelectMenu[i].x = kOriginalPosX;//元の位置に戻す
				SelectMenu[i].color = 0xaaa0ff;//元の色に戻す
			}
		}
	}
	
	//「次へ」ボタンが押されたら次シーンへ移行する
	if (input.IsTriggered(InputType::slect))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);

		m_updateFunc = &GameoverScene::FadeOutUpdat;
	}
}

void GameoverScene::NormalDraw()
{
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"ゲームオーバー",0xffffff);
}

void GameoverScene::MojiDraw()
{
	SetFontSize(kMojiSize);
	//DrawString(0, 0, L"ゲームオーバー", 0xffffff);
	for (int i = 0;i < kMojiNum;i++)
	{
		DrawStringF(m_moji[i].pos.x+2, m_moji[i].pos.y+2 + m_moji[i].moveY, kMoji[i], 0x000000);
		DrawStringF(m_moji[i].pos.x , m_moji[i].pos.y + m_moji[i].moveY, kMoji[i], 0xff0000);
	}
	SetFontSize(0);

	SetFontSize(kMenuFontSize);
	DrawString(SelectMenu[menuGameEnd].x + 5, SelectMenu[menuGameEnd].y + 5, L"タイトルに戻る", 0x000000);
	DrawString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, L"タイトルに戻る", SelectMenu[menuGameEnd].color);
	DrawString(SelectMenu[menuRestart].x + 5, SelectMenu[menuRestart].y + 5, L"最初から", 0x000000);
	DrawString(SelectMenu[menuRestart].x, SelectMenu[menuRestart].y, L"最初から", SelectMenu[menuRestart].color);
	SetFontSize(0);
}

void GameoverScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		switch (m_selectNum)
		{
		case menuGameEnd:
		default:
			m_manager.ChangeScene(new TitleScene(m_manager));
			return;
		}
	}
}

