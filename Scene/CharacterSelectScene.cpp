#include "CharacterSelectScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/Info.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Mouse.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "ExplanationScene.h"
#include "SettingScene.h"

namespace
{
	constexpr float kSizeScale = 4.0f;//表示拡大率

	constexpr int kCharSize = 32;//キャラクタのサイズ
	constexpr int kAnimSpeed = 5;//アニメーションスピード

	constexpr int kFontSize = 30;

	static constexpr int pw_width = 400;
	static constexpr int pw_height = 300;
	static constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	static constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;
	static constexpr int kPosY = (pw_start_y + pw_height) - (pw_height / 2);
}


CharacterSelectScene::CharacterSelectScene(SceneManager& manager) :
	Scene(manager), m_updateFunc(&CharacterSelectScene::FadeInUpdat), m_drawFunc(&CharacterSelectScene::NormalDraw), m_frameCount(0), m_select(-1), m_selectChar(),
	m_bgH(-1), m_scroll(0), isExpo(false)
{
	//キャラクター初期化
	int typeIdle = static_cast<int>(CharAnimType::Idle);
	int typeRun = static_cast<int>(CharAnimType::Run);
	int typeDoubleJump = static_cast<int>(CharAnimType::DoubleJump);

	float screneSizeW = Game::kScreenWidth / 2;
	float screneSizeH = Game::kScreenHeight / 2;

	m_char[static_cast<int>(CharType::MaskDude)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/MaskDude/Idle.png");
	m_char[static_cast<int>(CharType::MaskDude)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/MaskDude/Run.png");
	m_char[static_cast<int>(CharType::MaskDude)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/MaskDude/DoubleJump.png");
	m_char[static_cast<int>(CharType::MaskDude)].isLeft = false;
	m_char[static_cast<int>(CharType::MaskDude)].rect.center = { screneSizeW / 2 ,screneSizeH / 2 };

	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Idle.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/Run.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/NinjaFrog/DoubleJump.png");
	m_char[static_cast<int>(CharType::NinjaFrog)].isLeft = true;
	m_char[static_cast<int>(CharType::NinjaFrog)].rect.center = { Game::kScreenWidth - screneSizeW / 2 ,screneSizeH / 2 };

	m_char[static_cast<int>(CharType::PinkMan)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/PinkMan/Idle.png");
	m_char[static_cast<int>(CharType::PinkMan)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/PinkMan/Run.png");
	m_char[static_cast<int>(CharType::PinkMan)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/PinkMan/DoubleJump.png");
	m_char[static_cast<int>(CharType::PinkMan)].isLeft = false;
	m_char[static_cast<int>(CharType::PinkMan)].rect.center = { screneSizeW / 2 ,Game::kScreenHeight - screneSizeH / 2 - kFontSize * 2 };

	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeIdle] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Idle.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeRun] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/Run.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].handle[typeDoubleJump] = my::MyLoadGraph(L"Data/Characters/VirtualGuy/DoubleJump.png");
	m_char[static_cast<int>(CharType::VirtualGuy)].isLeft = true;
	m_char[static_cast<int>(CharType::VirtualGuy)].rect.center = { Game::kScreenWidth - screneSizeW / 2 ,Game::kScreenHeight - screneSizeH / 2 - kFontSize * 2 };

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
	//背景
	m_bgH = my::MyLoadGraph(L"Data/Background/Purple.png");
	//BGM
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/title.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	//メニュー
	m_menu[static_cast<int>(Item::Yes)].x = (pw_start_x + pw_width) - (pw_width / 2);
	m_menu[static_cast<int>(Item::Yes)].name = L"はい";

	m_menu[static_cast<int>(Item::No)].x = pw_start_x + kFontSize;
	m_menu[static_cast<int>(Item::No)].name = L"いいえ";

	for (auto& menu : m_menu)
	{
		menu.y = kPosY;
		menu.fontSize = kFontSize;
		menu.color = 0xffffff;
		menu.nameNum = static_cast<int>(wcslen(menu.name));
	}
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
CharacterSelectScene::Update(Mouse& mouse)
{
	(this->*m_updateFunc)(mouse);
	//背景移動
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
	//キャラクター
	for (auto& charctor : m_char)
	{
		int type = static_cast<int>(charctor.type);

		my::MyDrawRectRotaGraph(charctor.rect.center.x, charctor.rect.center.y,
			charctor.idxX * kCharSize, charctor.idxY * kCharSize, kCharSize, kCharSize,
			kSizeScale, 0.0f, charctor.handle[type], true, charctor.isLeft);
#ifdef _DEBUG
		charctor.rect.Draw(0xffffff);
#endif
	}

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void CharacterSelectScene::FadeInUpdat( Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);//画面のフェードイン
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);//音のフェードイン
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &CharacterSelectScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void CharacterSelectScene::NormalUpdat(Mouse& mouse)
{
	for (int i = 0; i < static_cast<int>(CharType::Max);i++)
	{
		//マウスがキャラクターの範囲内にあるとき
		if (mouse.MouseSelect(m_char[i].rect.center.x - m_char[i].rect.size.w / 2, m_char[i].rect.center.x + m_char[i].rect.size.w / 2, 
			m_char[i].rect.center.y - m_char[i].rect.size.h / 2, m_char[i].rect.center.y + m_char[i].rect.size.h / 2))
		{
			//走っているアニメーションに変更する
			if (m_char[i].type != CharAnimType::Run)
			{
				m_char[i].type = CharAnimType::Run;
				m_char[i].idxX = 0;
			}
			//マウスが押されたら次シーンへ移行する
			if (mouse.IsTrigger(Mouse::InputType::left))
			{
				SoundManager::GetInstance().Play(SoundId::Determinant);
				m_selectChar = i;
				m_updateFunc = &CharacterSelectScene::MoveChar;
			}
		}
		else
		{
			//待機アニメーションに
			if (m_char[i].type == CharAnimType::Idle)	continue;
			m_char[i].type = CharAnimType::Idle;
			m_char[i].idxX = 0;
		}
	}
	//アニメーション
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

//選択した後のキャラクターの動き
void CharacterSelectScene::MoveChar( Mouse& mouse)
{
	CharType type = static_cast<CharType>(m_selectChar);

	bool isOk = false;

	//一つアニメーションを再生する
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
		//どのシーンに行くかを選択するUpdateに
		m_updateFunc = &CharacterSelectScene::SelectScene;
		m_drawFunc = &CharacterSelectScene::SelectSceneDraw;
	}
}

//どのシーンに変更するかを決める
void CharacterSelectScene::SelectScene(Mouse& mouse)
{
	bool isSelect = false;
	m_select = -1;

	//マウスで選択
	int i = 0;
	for (auto& menu : m_menu)
	{
		if (mouse.MouseSelect(menu.x, menu.x + menu.fontSize * menu.nameNum,menu.y, menu.y + menu.fontSize))
		{
			if (m_select != i)
			{
				m_select = i;
			}
			isSelect = true;
		}
		i++;
	}

	int pauseMax = static_cast<int>(Item::Max);
	for (int i = 0; i < pauseMax; i++)
	{
		if (i == m_select)
		{
			m_menu[i].fontSize = kFontSize * 2;
			m_menu[i].color = 0xaaffaa;
		}
		else
		{
			m_menu[i].fontSize = kFontSize;
			m_menu[i].color = 0xffffff;
		}
	}

	if (isSelect && mouse.IsTrigger(Mouse::InputType::left))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_updateFunc = &CharacterSelectScene::FadeOutUpdat;
		if (m_select == static_cast<int>(Item::Yes))
		{
			isExpo = true;
		}
	}
}

void CharacterSelectScene::FadeOutUpdat( Mouse& mouse)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		//説明シーンかゲームプレイングシーンに変える
		if (isExpo)
		{
			m_manager.ChangeScene(new ExplanationScene(m_manager, m_selectChar));
		}
		else
		{
			m_manager.ChangeScene(new GameplayingScene(m_manager, m_selectChar));
		}
		return;
	}
}

void CharacterSelectScene::NormalDraw()
{
	DrawExplanationString(static_cast<int>(CharType::MaskDude), 0xb34607);
	DrawExplanationString(static_cast<int>(CharType::NinjaFrog), 0x136e1f);
	DrawExplanationString(static_cast<int>(CharType::PinkMan), 0xdb309d);
	DrawExplanationString(static_cast<int>(CharType::VirtualGuy), 0x09a8b0);
}

void CharacterSelectScene::SelectSceneDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//ウィンドウセロファン
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(kFontSize);
	int x = pw_width / 2 + pw_start_x - kFontSize * 4;
	DrawString(x, pw_start_y + 10, L"説明を聞きますか？", 0xffff88);
	
	for (auto& menu : m_menu)
	{
		SetFontSize(menu.fontSize);
		DrawString(menu.x, menu.y, menu.name, menu.color);
#ifdef _DEBUG
		int size = menu.fontSize * menu.nameNum;
		DrawBox(menu.x, menu.y, menu.x + size, menu.y + menu.fontSize, menu.color, false);
#endif
	}

	SetFontSize(0);
	//ウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}

void CharacterSelectScene::DrawExplanationString(int type, int color)
{
	//キャラクターの説明
	const wchar_t* string[] =
	{
		L"HPが\n多いよ",
		L"移動が\n速いよ",
		L"フルーツに\n当たりにくいよ",
		L"無敵時間が\n長いよ"
	};
	float x = m_char[type].rect.center.x - kCharSize * kSizeScale / 2;
	float y = m_char[type].rect.center.y + kCharSize * kSizeScale / 2;

	SetFontSize(kFontSize);
	DrawStringF(x, y, string[type], color);
	SetFontSize(0);
}
