#include "GameclearScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"


namespace
{
	constexpr float kMoveNum = 2.0f;//�v���C���[�ړ��X�s�[�h

	constexpr int kMojiSize = 90;//�����̑傫��

	//�v���C���[�O���t�B�b�N
	constexpr int kGraphSizeWidth = 32;		//�T�C�Y
	constexpr int kGraphSizeHeight = 32;	//�T�C�Y
	constexpr float kDrawScale = 3.5f;		//�g�嗦
	constexpr int kAnimNum = 5;		//�A�j���[�V��������
	constexpr int kFrameSpeed = 10;		//�A�j���[�V�����X�s�[�h

}

GameclearScene::GameclearScene(SceneManager& manager) :
	Scene(manager), m_updateFunc(&GameclearScene::FadeInUpdat),
	m_drawFunc(&GameclearScene::NormalDraw) {
	
	float posX = (Game::kScreenWidth - kMojiNum * kMojiSize) / 2;
	for (int i = 0; i < kMojiNum; i++)
	{
		m_moji[i].pos = { static_cast<float>(posX + i * kMojiSize) ,Game::kScreenHeight / 3 };
		m_moji[i].moveY = i * -1.0f;
		m_moji[i].add = 0.5f;
	}
	
	//m_BgmH = LoadSoundMem(L"Sound/BGM/emerald.mp3");
}

GameclearScene::~GameclearScene()
{
	DeleteSoundMem(m_BgmH);
	SoundManager::GetInstance().StopBgm(SoundId::EnemyShot);
}

void GameclearScene::Update(const InputState& input,  Mouse& mouse)
{
	(this->*m_updateFunc)(input,mouse);
}

void GameclearScene::Draw()
{
	(this->*m_drawFunc)();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameclearScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameclearScene::NormalUpdat;
		m_fadeValue = 0;
		SoundManager::GetInstance().Play(SoundId::Gameclear);
	}
}
void GameclearScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}

void GameclearScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	if (input.IsTriggered(InputType::slect))
	{
		m_updateFunc = &GameclearScene::MojiUpdate;
		m_drawFunc = &GameclearScene::MojiDraw;
		ChangeVolumeSoundMem(0, m_BgmH);
		PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
		return;
	}
}

void GameclearScene::MojiUpdate(const InputState& input,  Mouse& mouse)
{
	if (m_soundVolume++ >= SoundManager::GetInstance().GetBGMVolume())
	{
		m_soundVolume = SoundManager::GetInstance().GetBGMVolume();
	}
	ChangeVolumeSoundMem(m_soundVolume, m_BgmH);

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

	if (input.IsTriggered(InputType::slect))
	{
		m_updateFunc = &GameclearScene::FadeOutUpdat;
		m_fadeColor = 0x000000;
	}
}

void GameclearScene::NormalDraw()
{
	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"�Q�[���N���A", 0xffffff);
}

void GameclearScene::MojiDraw()
{
	SetFontSize(kMojiSize);
	for (int i = 0; i < kMojiNum; i++)
	{
		DrawStringF(m_moji[i].pos.x + 2, m_moji[i].pos.y + 2 + m_moji[i].moveY, kMoji[i], 0xffffff);
		DrawStringF(m_moji[i].pos.x, m_moji[i].pos.y + m_moji[i].moveY, kMoji[i], 0xffaaaf);
	}
	SetFontSize(0);
}
