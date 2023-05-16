#include "GameclearScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/Mouse.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../Game/Character.h"


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

GameclearScene::GameclearScene(SceneManager& manager, std::shared_ptr<Character> character) :
	Scene(manager), m_updateFunc(&GameclearScene::FadeInUpdat),m_char(character), m_scroll(0)
{
	//m_BgmH = LoadSoundMem(L"Sound/BGM/emerald.mp3");
	m_bgH = my::MyLoadGraph(L"Data/Background/Gray.png");
}

GameclearScene::~GameclearScene()
{
	DeleteSoundMem(m_BgmH);
	SoundManager::GetInstance().StopBgm(SoundId::EnemyShot);
}

void GameclearScene::Update(const InputState& input,  Mouse& mouse)
{
	m_char->Update(true);
	(this->*m_updateFunc)(input,mouse);
	//�w�i�ړ�
	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}
}

void GameclearScene::Draw()
{
	//�w�i
	for (int x = -kBgSize / 2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize / 2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x + m_scroll, y + m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	m_char->Draw();

	DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 2, L"�Q�[���N���A", 0xffffff);

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
		m_updateFunc = &GameclearScene::FadeOutUpdat;
		m_fadeColor = 0x000000;
		ChangeVolumeSoundMem(0, m_BgmH);
		PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
		return;
	}
}

