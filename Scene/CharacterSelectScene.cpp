#include "CharacterSelectScene.h"
#include <DxLib.h>

#include "../game.h"
#include "../InputState.h"

#include "../Util/Sound.h"
#include "../Util/Info.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Mouse.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "MonologueScene.h"
#include "SettingScene.h"

namespace
{
	constexpr float kSizeScale = 4.0f;//表示拡大率

	constexpr int kCharSize = 32;//キャラクタのサイズ
	constexpr int kAnimSpeed = 5;//アニメーションスピード
}


CharacterSelectScene::CharacterSelectScene(SceneManager& manager) : 
	Scene(manager), m_updateFunc(&CharacterSelectScene::FadeInUpdat),m_frameCount(0), m_selectChar(), m_bgH(-1),m_scroll(0)
{
	//キャラクター初期化
	int typeIdle = static_cast<int>(CharAnimType::Idle);
	int typeRun = static_cast<int>(CharAnimType::Run);
	int typeJump = static_cast<int>(CharAnimType::Jump);
	int typeDoubleJump = static_cast<int>(CharAnimType::DoubleJump);
	int typeFall = static_cast<int>(CharAnimType::Fall);

	float screneSizeW = Game::kScreenWidth / 2;
	float screneSizeH = Game::kScreenHeight / 2;

	m_char[static_cast<int>(CharType::MaskDude)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/MaskDude/Idle.png");
	m_char[static_cast<int>(CharType::MaskDude)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/MaskDude/Run.png");
	m_char[static_cast<int>(CharType::MaskDude)].handle[typeJump] = my::MyLoadGraph(L"Data/Characters/MaskDude/Jump.png");
	m_char[static_cast<int>(CharType::MaskDude)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/MaskDude/DoubleJump.png");
	m_char[static_cast<int>(CharType::MaskDude)].handle[typeFall] = my::MyLoadGraph(L"Data/Characters/MaskDude/Fall.png");
	m_char[static_cast<int>(CharType::MaskDude)].isLeft = false;
	m_char[static_cast<int>(CharType::MaskDude)].rect.center = {screneSizeW / 2 ,screneSizeH / 2};

	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Idle.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Run.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeJump] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Jump.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/DoubleJump.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeFall] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Fall.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].isLeft = true;
	m_char[static_cast<int>(CharType::NinjaFrog)].rect.center = { Game::kScreenWidth - screneSizeW / 2 ,screneSizeH / 2 };

	m_char[static_cast<int>(CharType::PinkMan)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/PinkMan/Idle.png");
	m_char[static_cast<int>(CharType::PinkMan)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/PinkMan/Run.png");
	m_char[static_cast<int>(CharType::PinkMan)].handle[typeJump] = my::MyLoadGraph(L"Data/Characters/PinkMan/Jump.png");
	m_char[static_cast<int>(CharType::PinkMan)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/PinkMan/DoubleJump.png");
	m_char[static_cast<int>(CharType::PinkMan)].handle[typeFall] = my::MyLoadGraph(L"Data/Characters/PinkMan/Fall.png");
	m_char[static_cast<int>(CharType::PinkMan)].isLeft = false;
	m_char[static_cast<int>(CharType::PinkMan)].rect.center = { screneSizeW / 2 ,Game::kScreenHeight - screneSizeH / 2 };

	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Idle.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Run.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeJump] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Jump.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/DoubleJump.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeFall] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Fall.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].isLeft = true;
	m_char[static_cast<int>(CharType::VirtualGuy)].rect.center = { Game::kScreenWidth - screneSizeW / 2 ,Game::kScreenHeight - screneSizeH / 2 };

	int  size = static_cast<int>(kCharSize * kSizeScale);

	for (auto& charctor : m_char)
	{
		for (int i = 0; i < static_cast<int>(CharAnimType::Max); i++)
		{
			GetGraphSize(charctor.handle[i], &charctor.w[i], &charctor.h);

			charctor.w[i] = charctor.w[i] / kCharSize;
		}

		charctor.type = CharAnimType::Idle;

		charctor.rect.size = { size,size };

		charctor.idxX = 0;
		charctor.idxY = 0;
	}

	m_bgH = my::MyLoadGraph(L"Data/Background/Gray.png");

	/*m_BgmH = LoadSoundMem(L"Sound/BGM/noranekonokuchibue.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);*/
}

CharacterSelectScene::~CharacterSelectScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_bgH);

	for (auto& charctor : m_char)
	{
		for (auto& anim : charctor.handle)
		{
			DeleteGraph(anim);
		}
	}
}

void
CharacterSelectScene::Update(const InputState& input, Mouse& mouse)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input,mouse);
	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}
}

void CharacterSelectScene::Draw()
{
	//背景
	for (int x = -kBgSize / 2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize / 2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x+ m_scroll, y+ m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	for (auto& charctor : m_char)
	{
		int type = static_cast<int>(charctor.type);

		my::MyDrawRectRotaGraph(charctor.rect.center.x, charctor.rect.center.y,
			charctor.idxX * kCharSize, charctor.idxY * kCharSize, kCharSize, kCharSize,
			kSizeScale, 0.0f, charctor.handle[type], true, charctor.isLeft);

		charctor.rect.Draw(0xffffff);
	}

#ifdef _DEBUG
	
#endif
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void CharacterSelectScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &CharacterSelectScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void CharacterSelectScene::NormalUpdat(const InputState& input,  Mouse& mouse)
{
	//マウスがキャラクターの範囲内にあるとき
	for (int i = 0; i < static_cast<int>(CharType::Max);i++)
	{
		if (mouse.MouseSelect(m_char[i].rect.center.x - m_char[i].rect.size.w / 2, m_char[i].rect.center.x + m_char[i].rect.size.w / 2, 
			m_char[i].rect.center.y - m_char[i].rect.size.h / 2, m_char[i].rect.center.y + m_char[i].rect.size.h / 2))
		{
			if (m_char[i].type != CharAnimType::Run)
			{
				m_char[i].type = CharAnimType::Run;
				m_char[i].idxX = 0;
			}
			//メニュー
			//「次へ」ボタンが押されたら次シーンへ移行する
			if (input.IsTriggered(InputType::slect))
			{
				SoundManager::GetInstance().Play(SoundId::Determinant);
				m_selectChar = i;
				m_updateFunc = &CharacterSelectScene::MoveChar;
			}
		}
		else
		{
			if (m_char[i].type == CharAnimType::Idle)	continue;
			m_char[i].type = CharAnimType::Idle;
			m_char[i].idxX = 0;
		}
	}
	
	if (m_frameCount++ >= kAnimSpeed)
	{
		m_frameCount = 0;
		for (auto& charctor : m_char)
		{
			charctor.idxX++;

			int type = static_cast<int>(charctor.type);

			if (charctor.idxX >= charctor.w[type])
			{
				charctor.idxX -= charctor.w[type];
			}
		}
	}
}

void CharacterSelectScene::MoveChar(const InputState& input, Mouse& mouse)
{
	CharType type = static_cast<CharType>(m_selectChar);

	bool isOk = false;

	if (m_char[m_selectChar].type != CharAnimType::DoubleJump)
	{
		m_char[m_selectChar].type = CharAnimType::DoubleJump;
		m_char[m_selectChar].idxX = 0;
	}
	m_frameCount += 2;

	if (m_frameCount >= kAnimSpeed)
	{
		m_frameCount = 0;
		m_char[m_selectChar].idxX++;

		int type = static_cast<int>(m_char[m_selectChar].type);

		if (m_char[m_selectChar].idxX >= m_char[m_selectChar].w[type])
		{
			m_char[m_selectChar].idxX -= m_char[m_selectChar].w[type];
			isOk = true;
			m_char[m_selectChar].type = CharAnimType::Idle;
			m_char[m_selectChar].idxX = 0;
		}
	}

	if (isOk) 
	{
		m_updateFunc = &CharacterSelectScene::FadeOutUpdat; 
	}
}

void CharacterSelectScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		//ゲームシーンに変更する
		m_manager.ChangeScene(new GameplayingScene(m_manager, m_selectChar));
		return;
	}
}
