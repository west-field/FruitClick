
#include "GameplayingScene.h"

#include "DxLib.h"
#include <cassert>

#include "SceneManager.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "GameendScene.h"

#include "../game.h"
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
	constexpr int kFruitsCreateMinFrame = 20;//フルーツを作る最小待ち時間

	constexpr float kSettingDrawScale = 2.0f;//設定ボタン表示拡大率

	constexpr int kFontWidth = 16;//数字の画像の横幅
	constexpr int kFontHeight = 32;//数字の画像の縦幅

	constexpr int kBlockSpeed = 5;//箱の画像カウント
}

GameplayingScene::GameplayingScene(SceneManager& manager, int selectChar) :
	Scene(manager), m_updateFunc(&GameplayingScene::FadeInUpdat), m_prevFuruitsType(-1)
{
	//キャラクター
	m_char = std::make_shared<Character>(selectChar, Position2{ static_cast<float>(Game::kScreenWidth / 2),static_cast<float>(Game::kScreenHeight - 100) });
	//フルーツ生成
	m_fruitsFactory = std::make_shared<FruitsFactory>();
	
	m_startTime = GetNowCount();//現在の経過時間を得る
	m_fruitsFrame = kFruitsCreateFrame;//フルーツ生成までの時間

	//設定ボタン
	m_settingH = my::MyLoadGraph(L"Data/UI/Settings.png");
	int X = 0, Y = 0;
	GetGraphSize(m_settingH, &X, &Y);
	X = static_cast<int>(X * kSettingDrawScale);
	Y = static_cast<int>(Y * kSettingDrawScale);
	m_settingRect = { {static_cast<float>(Game::kScreenWidth - X / 2),static_cast<float>(Y / 2)}, {X,Y} };

	//背景
	m_bgH = my::MyLoadGraph(L"Data/Background/Gray.png");
	m_scroll = 0;
	//BGM
	m_BgmH = LoadSoundMem(L"Data/Sound/BGM/gameMain.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
	//得点
	m_numFont = my::MyLoadGraph(L"Data/numfont.png");
	//箱
	m_boxes[0].handle = my::MyLoadGraph(L"Data/block.png");
	m_boxes[1].handle = my::MyLoadGraph(L"Data/blockHit.png");

	m_boxes[0].animNum = 1;
	m_boxes[1].animNum = 4;

	for (auto& block : m_boxes)
	{
		block.sizeW = 28;
		block.sizeH = 24;
	}
	m_type = 0;//箱表示タイプ
	m_idx = 0;//箱のアニメーション

	m_isCountDown = true;//最初にカウントダウン
	m_count = 3 * 60;//カウント
}

GameplayingScene::~GameplayingScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_settingH);
	DeleteGraph(m_bgH);
	DeleteGraph(m_numFont);

	for (auto& block : m_boxes)
	{
		DeleteGraph(block.handle);
	}
}

void GameplayingScene::Update( Mouse& mouse)
{
	(this->*m_updateFunc)(mouse);
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
	//箱
	int y = 2 * kBgSize;
	for (int i = 0; i < 6; i++)
	{
		int x = (i + 2) * kBgSize + kBgSize / 2;
		my::MyDrawRectRotaGraph(x, y, m_idx * m_boxes[m_type].sizeW, 0,
			m_boxes[m_type].sizeW, m_boxes[m_type].sizeH, 2.0f, 0.0f, m_boxes[m_type].handle, true, false);
	}

	m_fruitsFactory->Draw();//フルーツ
	m_char->Draw(true);//キャラ

	for (auto& point : m_fruitsPoint)
	{
		if (!point.isExist)	continue;//存在しないときは処理をしない
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, point.fade);
		PointUpdate(static_cast<int>(point.pos.x), static_cast<int>(point.pos.y), point.point);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	PointUpdate(Game::kScreenWidth / 2, kFontHeight, m_point);//ポイント

	//設定画像描画
	my::MyDrawRectRotaGraph(m_settingRect.center.x, m_settingRect.center.y,
		0, 0, m_settingRect.size.w, m_settingRect.size.h,
		kSettingDrawScale, 0.0f, m_settingH, true, false);

	//最初のカウントダウン
	if (m_isCountDown)
	{
		//0の時スタートを表示する
		if (m_count / 60 == 0)
		{
			SetFontSize(40);
			DrawFormatStringF(Game::kScreenWidth / 2 - 5*40/2, Game::kScreenHeight / 2, 0x000000, L"スタート!!");
			SetFontSize(0);
		}
		else
		{
			SetFontSize(40);
			DrawFormatStringF(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0x000000, L"%d", m_count / 60);
			SetFontSize(0);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameplayingScene::FadeInUpdat(Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);//音のフェードイン
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameplayingScene::CountDownUpdate;
		SoundManager::GetInstance().Play(SoundId::Count);
		m_fadeValue = 0;
		return;
	}
}

void GameplayingScene::CountDownUpdate(Mouse& mouse)
{
	m_count--;
	
	if (m_count == 0)
	{
		m_count = 0;
		m_updateFunc = &GameplayingScene::NormalUpdat;
		m_isCountDown = false;
		return;
	}
	//カウントが60になったらスタート音を鳴らす
	if (m_count == 60)
	{
		SoundManager::GetInstance().Play(SoundId::Start);
	}
	//カウントが0の時カウント音を鳴らす
	else if (m_count % 60 == 0)
	{
		SoundManager::GetInstance().Play(SoundId::Count);
	}
}

void GameplayingScene::NormalUpdat(Mouse& mouse)
{
	//フルーツから出るポイント
	for (auto& point : m_fruitsPoint)
	{
		if (!point.isExist)	continue;//存在しないときは処理をしない
		point.fade -= 5;
		point.pos.y -= 1.0f;//上に上がる
		if (point.count-- <= 0)//カウントが0になった時
		{
			point.isExist = false;//存在を消す
		}
	}
	//いらなくなったものを削除(フルーツから出るポイント)
	m_fruitsPoint.remove_if([](Point point) {
		return !point.isExist;
		});
	//ポイントのカウントアップ
	if (m_pointCount-- <= 0)
	{
		if (m_pointAdd != 0)
		{
			m_pointAdd--;
			m_point++;
			SoundManager::GetInstance().Play(SoundId::Point);//ポイント取得音を鳴らす
		}
		m_pointCount = 5;
	}
	
	m_char->Update();//キャラクター
	m_fruitsFactory->Update();//フルーツ生成

	//背景スクロール
	if (m_scroll++ >= static_cast<int>(kBgSize))
	{
		m_scroll -= static_cast<int>(kBgSize);
	}

	//フルーツ生成
	if (m_fruitsFrame-- <= 0)
	{		
		int now = (GetNowCount() - m_startTime) / 1000;//経過時間

		//経過時間が待ち時間-最小待ち時間よりも大きかったら
		if (now > kFruitsCreateFrame - kFruitsCreateMinFrame)
		{
			//待ち時間-最小待ち時間以上にならないようにする。
			now = kFruitsCreateFrame - kFruitsCreateMinFrame;
		}
		//待ち時間から経過時間を引くことでだんだん生成速度を上げる
		m_fruitsFrame = kFruitsCreateFrame - now;
		SpawnerUpdate();
	}

	//箱
	if (m_count-- <= 0)
	{
		if (m_idx++ >= m_boxes[m_type].animNum - 1)
		{
			m_idx -= m_boxes[m_type].animNum;
			if (m_type == 1)
			{
				m_type = 0;
				m_idx = 0;
			}
		}

		m_count = kBlockSpeed;
	}

	//マウスとフルーツの当たり判定
	//マウスを押したとき
	if (mouse.IsTrigger(Mouse::InputType::left))
	{
		//フルーツ
		for (auto& fruit : m_fruitsFactory->GetFruits())
		{
			if (!fruit->IsExist()) continue;//フルーツが存在するかどうか
			//フルーツの位置にマウスがあるとき
			if (fruit->GetRect().IsHit(Rect{ {mouse.GetMousePos()},{10,10} }))
			{
				SoundManager::GetInstance().Play(SoundId::FruitClick);
				//フルーツにダメージ
				fruit->OnDamage(1);
				if (fruit->GetHp() <= 0)
				{
					fruit->SetDestroy();
					m_pointAdd += fruit->GetPoint();
					Point p =
					{
						fruit->GetPoint(),
						fruit->GetRect().GetCenter(),
						true
					};
					m_fruitsPoint.push_back(p);
				}
				continue;
			}
		}
	}

	//キャラクタとフルーツの当たり判定
	for (auto& fruit : m_fruitsFactory->GetFruits())
	{
		if (!fruit->IsExist()) continue;//存在しない場合

		if (fruit->GetRect().IsHit(m_char->GetRect()))
		{
			//どんな時でもフルーツがキャラクタに当たったら消える
			fruit->SetExist(false);
			
			if (!m_char->IsCollidable())	continue;//無敵時間の時は当たらない
			m_char->Damage(1);
			continue;
		}
	}

	//ポーズ画面
	if (mouse.GetRect().IsHit(m_settingRect))
	{
		if (mouse.IsTrigger(Mouse::InputType::left))
		{
			SoundManager::GetInstance().Play(SoundId::Determinant);
			int sound = m_BgmH;
			m_manager.PushScene(new PauseScene(m_manager, sound));
			return;
		}
	}

	//終了判定
	if (m_char->GetHp() <= 0)
	{
		if (m_pointAdd != 0)
		{
			m_point += m_pointAdd;
		}
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xffffff;//フェード時の色を白にする。
		return;
	}

	//右クリックしたときポーズ画面を表示する
	if (mouse.IsTrigger(Mouse::InputType::right))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_manager.PushScene(new PauseScene(m_manager, m_BgmH));
		return;
	}
}

void GameplayingScene::FadeOutUpdat( Mouse& mouse)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);

	if(++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new GameendScene(m_manager, m_char, m_point));
		return;
	}
}

void GameplayingScene::SpawnerUpdate()
{
	int fruitsSpawnIdRand = GetRand((static_cast<int>(FruitsSpawnId::Max) - 1));//ランダムで生成するフルーツの種類を決める
	int rand = GetRand(100) % 6;//ランダムにスポナー生成位置を決める
	assert(rand < 6);//スポナーの数よりも大きかったら止める
	//一つ前の生成場所と同じだった場合もう一度決めなおす
	while (m_prevFuruitsType == rand)
	{
		rand = GetRand(100) % 6;
		assert(rand < 6);
	}
	m_prevFuruitsType = rand;
	Position2 pos = { static_cast<float>((rand + 2) * kBgSize) + kBgSize / 2, static_cast<float>(2 * kBgSize) };

	FruitsCreate(static_cast<FruitsSpawnId>(fruitsSpawnIdRand), pos);//フルーツを生成する
	SoundManager::GetInstance().Play(SoundId::FruitCreate);//生成音を鳴らす
	m_type = 1;
	m_idx = 0;

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

