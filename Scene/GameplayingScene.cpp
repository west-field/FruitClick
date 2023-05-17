
#include "GameplayingScene.h"

#include "DxLib.h"
#include <cassert>

#include "SceneManager.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "GameoverScene.h"
#include "GameclearScene.h"

#include "../game.h"
#include "../InputState.h"
#include "../Util/Sound.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Mouse.h"

#include "../Game/Character.h"
#include "../Game/Stage.h"
#include "../Game/FruitsFactory.h"
#include "../Game/Fruits/FruitsBase.h"

#include "../Game/StraightSpawner.h"

namespace
{
	constexpr float kPlayerMoveSpeed = 5.0f;//プレイヤーの移動速度

	constexpr int kFruitsCreateFrame = 100;//フルーツを作る待ち時間

	constexpr float kGearScale = 2.0f;//設定ボタン表示拡大率

	constexpr int kFontWidth = 16;
	constexpr int kFontHeight = 32;
}

GameplayingScene::GameplayingScene(SceneManager& manager, int selectChar) :
	Scene(manager), m_updateFunc(&GameplayingScene::FadeInUpdat)
{
	m_char = std::make_shared<Character>(selectChar, Position2{ static_cast<float>(Game::kScreenWidth / 2),static_cast<float>(Game::kScreenHeight - 100) });
	m_fruitsFactory = std::make_shared<FruitsFactory>();
	
	m_settingH = my::MyLoadGraph(L"Data/Buttons/Settings.png");
	int X = 0, Y = 0;
	GetGraphSize(m_settingH, &X, &Y);
	X = static_cast<int>(X * kGearScale);
	Y = static_cast<int>(Y * kGearScale);
	m_settingRect = { {static_cast<float>(Game::kScreenWidth - X / 2),static_cast<float>(Y / 2)}, {X,Y} };

	m_stage = std::make_shared<Stage>();
	m_stage->Load(L"Data/map.fmf");

	m_bgH = my::MyLoadGraph(L"Data/Background/Gray.png");
	m_scroll = 0;
	//BGM
	//m_BgmH = LoadSoundMem(L"Sound/BGM/Disital_Delta.mp3");
	//m_bossBgm = LoadSoundMem(L"Sound/BGM/arabiantechno.mp3");
	//ChangeVolumeSoundMem(0, m_BgmH);
	//PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	m_numFont = my::MyLoadGraph(L"Data/numfont.png");
}

GameplayingScene::~GameplayingScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteSoundMem(m_bossBgm);
	DeleteGraph(m_settingH);
	DeleteGraph(m_bgH);
	DeleteGraph(m_numFont);
}

void GameplayingScene::Update(const InputState& input,  Mouse& mouse)
{
	(this->*m_updateFunc)(input,mouse);
}

void GameplayingScene::Draw()
{
	//背景
	for (int x = -kBgSize / 2; x < Game::kScreenWidth; x += kBgSize)
	{
		for (int y = -kBgSize / 2; y <= Game::kScreenHeight; y += kBgSize)
		{
			my::MyDrawRectRotaGraph(x+m_scroll, y+ m_scroll, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_bgH, true, false);
		}
	}

	m_char->Draw();
	m_fruitsFactory->Draw();

	PointUpdate(Game::kScreenWidth / 2, kFontHeight, m_fruitsFactory->GetPoint());

#ifdef _DEBUG
	m_stage->Draw();
	DrawString(0, 20, L"ゲームプレイングシーン", 0xffffff);
	DrawFormatString(0, 100,0x000000, L"壊した数:%d", m_fruitsFactory->GetPoint());
#endif
	//メニュー項目を描画
	my::MyDrawRectRotaGraph(m_settingRect.center.x, m_settingRect.center.y,
		0, 0, m_settingRect.size.w, m_settingRect.size.h,
		kGearScale, 0.0f, m_settingH, true, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameplayingScene::FadeInUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameplayingScene::NormalUpdat;
		m_fadeValue = 0;
		return;
	}
}

void GameplayingScene::NormalUpdat(const InputState& input, Mouse& mouse)
{
	m_char->Update();
	m_fruitsFactory->Update();
	//背景スクロール
	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}

	//フルーツ生成
	if (m_fruitsFrame++ >= kFruitsCreateFrame)
	{		
		m_fruitsFrame = 0;
		SpawnerUpdate();
	}

	//マウスとフルーツの当たり判定
	//マウスを押したとき
	if (mouse.IsTriggerLeft())
	{
		//フルーツ
		for (auto& fruit : m_fruitsFactory->GetFruits())
		{
			if (!fruit->IsExist()) continue;//フルーツが存在するかどうか
			//フルーツの位置にマウスがあるとき
			if (fruit->GetRect().IsHit(Rect{ {mouse.GetPos()},{10,10} }))
			{
				//フルーツにダメージ
				fruit->OnDamage(1);
				if (fruit->GetHp() <= 0)
				{
					fruit->SetDestroy();
				}
				continue;
			}
		}
	}

	//キャラクタとフルーツの当たり判定
	for (auto& fruit : m_fruitsFactory->GetFruits())
	{
		if (!fruit->IsExist()) continue;//存在しない場合
		if (!fruit->IsCollidable()) continue;//当たるかどうかの判定

		if (fruit->GetRect().IsHit(m_char->GetRect()))
		{
			fruit->SetExist(false);
			m_char->Damage(1);
			continue;
		}
	}

	//ポーズ画面
	if (mouse.GetRect().IsHit(m_settingRect))
	{
		if (input.IsTriggered(InputType::slect))
		{
			SoundManager::GetInstance().Play(SoundId::MenuOpen);
			int sound = m_BgmH;
			m_manager.PushScene(new PauseScene(m_manager, sound));
			return;
		}
	}
	if (input.IsTriggered(InputType::pause))
	{
		SoundManager::GetInstance().Play(SoundId::MenuOpen);
		int sound = m_BgmH;
		m_manager.PushScene(new PauseScene(m_manager, sound));
		return;
	}

	//終了判定
	if (m_char->GetHp() <= 0)
	{
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xffffff;//フェード時の色を白にする。
		return;
	}

#ifdef _DEBUG

	if (mouse.IsPressLeft())
	{
		if (input.IsPressed(InputType::prev))
		{
			m_char->Damage(1);
		}
	}

#endif
}

void GameplayingScene::FadeOutUpdat(const InputState& input,  Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_bossBgm);

	if(++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new GameclearScene(m_manager, m_char, m_fruitsFactory->GetPoint()));
		return;
	}
}

void GameplayingScene::SpawnerUpdate()
{
	int fruitsSpawnIdRand = GetRand((static_cast<int>(FruitsSpawnId::Max) - 1));//ランダムで生成するフルーツの種類を決める
	int rand = GetRand(100) % m_stage->GetMapSpawnerNum();//ランダムにスポナー生成位置を決める
	assert(rand < m_stage->GetMapSpawnerNum());//スポナーの数よりも大きかったら止める
	Position2 pos = { static_cast<float>((rand + 2) * kBgSize) + kBgSize / 2, static_cast<float>(2 * kBgSize) };

	FruitsCreate(static_cast<FruitsSpawnId>(fruitsSpawnIdRand), pos);

#ifdef _DEBUG
	DrawBoxAA(pos.x, pos.y, pos.x + 64, pos.y + 64, 0x000000, false);
#endif

	for (auto& spawner : m_spawners)
	{
		spawner->Update();
	}
	//いらなくなったスポナーを削除
	m_spawners.remove_if([](const std::shared_ptr<Spawner> spawner) {
		return !spawner->IsExist();
		});
}

void GameplayingScene::FruitsCreate(FruitsSpawnId id, const Position2 pos)
{
	switch (id)
	{
	case FruitsSpawnId::AppleStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Apple,
				m_fruitsFactory
				));
		break;
	case FruitsSpawnId::BananaStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Banana,
				m_fruitsFactory
				));
		break;
	case FruitsSpawnId::CherrieStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Cherrie,
				m_fruitsFactory
				));
		break;
	case FruitsSpawnId::KiwiStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Kiwi,
				m_fruitsFactory
				));
		break;
	case FruitsSpawnId::MelonStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Melon,
				m_fruitsFactory
				));
		break;
	case FruitsSpawnId::OrangeStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Orange,
				m_fruitsFactory
				));
		break;
	case FruitsSpawnId::PineappleStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Pineapple,
				m_fruitsFactory
				));
		break;
	case FruitsSpawnId::StrawberryStraight:
		m_spawners.push_back(
			std::make_shared<StraightSpawner>(
				pos,
				FruitsType::Strawberry,
				m_fruitsFactory
				));
		break;
	default:
		break;
	}
}

void GameplayingScene::PointUpdate(int leftX, int y, int dispNum, int digit)
{
	int digitNum = 0;
	int temp = dispNum;
	int cutNum = 10;	// 表示桁数指定時に表示をおさめるために使用する

	// 表示する数字の桁数数える
	while (temp > 0)
	{
		digitNum++;
		temp /= 10;
		cutNum *= 10;
	}
	if (digitNum <= 0)
	{
		digitNum = 1;	// 0の場合は1桁表示する
	}

	// 表示桁数指定
	temp = dispNum;
	if (digit >= 0)
	{
		if (digitNum > digit)
		{
			// 下から指定桁数を表示するためはみ出し範囲を切り捨て
			temp %= cutNum;
		}
		digitNum = digit;
	}
	// 一番下の桁から表示
	int posX = leftX - kFontWidth;
	int posY = y;
	for (int i = 0; i < digitNum; i++)
	{
		int no = temp % 10;

		DrawRectGraph(posX, posY,
			no * 16, 0, 16, 32,
			m_numFont, true);

		temp /= 10;
		posX -= 16;
	}
}

