#include "ExplanationScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/DrawFunctions.h"

#include "SceneManager.h"
#include "GameplayingScene.h"

namespace
{
	constexpr int kButtonFontSize = 60;//フォントサイズ

	constexpr int kButtonSize = 480;//ボタン一つのサイズ
	constexpr float kButtonDrawScale = 0.2f;//拡大率
	constexpr int kButtonDrawSpeed = 10;//画像速度
	//プレイヤーグラフィック
	constexpr int kGraphSizeWidth = 32;		//サイズ
	constexpr int kGraphSizeHeight = 32;	//サイズ
	constexpr float kDrawScale = 2.0f;		//拡大率
	constexpr int kFrameSpeed = 10;		//アニメーションスピード
}

ExplanationScene::ExplanationScene(SceneManager& manager,int selectChar) : Scene(manager),
m_updateFunc(&ExplanationScene::FadeInUpdat), m_selectChar(selectChar)
{
	/*m_BgmH = LoadSoundMem(L"Sound/BGM/noranekonokuchibue.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);*/
}

ExplanationScene::~ExplanationScene()
{
	DeleteSoundMem(m_BgmH);
}

void
ExplanationScene::Update(const InputState& input,  Mouse& mouse)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input,mouse);
}

void ExplanationScene::Draw()
{

	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"説明", 0xffffff);

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
	if (input.IsPressed(InputType::slect))
	{
		m_updateFunc = &ExplanationScene::FadeOutUpdat;
		return;
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
