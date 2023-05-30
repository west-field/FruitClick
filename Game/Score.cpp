#include "Score.h"
#include <DxLib.h>
#include <cassert>
#include "../game.h"

namespace
{
	const char* path = "Data/score.info";

	constexpr int add = 5;
	constexpr int pw_width = 400;
	constexpr int pw_height = 300 + add * 5;
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;//左
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 5;//上

	constexpr int kPosX = pw_start_x + 10;
	constexpr int kPosY = pw_start_y + 10;

	constexpr int kFontSize = 20;
}

Score::Score():m_point(), m_header(), m_isSave(false)
{
	//PointInit();//ランキング初期化
}

Score::~Score()
{

}

void Score::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);//ウィンドウセロファン
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	SetFontSize(kFontSize * 2);
	int X = kPosX + ((kFontSize * 2) * 2);
	DrawString(X, kPosY, L"ランキング", 0xffff88);//メッセージ
	int y = kFontSize * 2 + 10;
	int color = 0xffffff;
	for (auto& score : m_header)
	{
		color = 0xffffff;
		if (m_isSave && score.point == m_point)
		{
			color = 0xEEE8AA;
		}
		DrawFormatString(X, kPosY + y, color, L"%d位：%d点", static_cast<int>(score.rank) + 1, score.point);
		y += kFontSize * 2 + add;
	}
	if (!m_isSave)
	{
		DrawFormatString(kPosX+ (kFontSize), kPosY + y, 0xEEE8AA, L"ランキング外:%d点", m_point);
	}
	SetFontSize(0);
	
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);//ウィンドウ枠線
}

void Score::Load()
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "rb");//ファイルを開く
	assert(fp);//ファイルを開けなかったら止める
	fread(&m_header, sizeof(m_header), 1, fp);//取得する

	fclose(fp);//ファイルを閉じる
}

void Score::Comparison(int score)
{
	if (m_point != score) { m_point = score; }
	m_isSave = false;
	//今回のポイントが一番下の取得したデータの得点よりも大きいとき
	if (score >= m_header[static_cast<int>(Rank::Max) - 1].point)
	{
		m_isSave = true;
		m_header[static_cast<int>(Rank::Max) - 1].point = score;
	}
	else
	{
		//一番したの得点よりも小さいときはここで終了する。
		return;
	}

	//下から順に5→4→3→2→1
	for (int i = static_cast<int>(Rank::Max) - 1; i >= 0; i--)
	{
		if (i - 1 < 0)	continue;
		//今回のポイントが取得した得点よりも大きいとき
		if ( m_header[i].point > m_header[i - 1].point)
		{
			int temp;
			temp = m_header[i].point;
			m_header[i].point = m_header[i - 1].point;
			m_header[i - 1].point = temp;
		}
	}
	//情報を変更した場合
	if (m_isSave)
	{
		Save();//得点データを書き込む
	}
}

void Score::Save()
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "wb");
	assert(fp);//ファイルを開けなかったら止める

	fwrite(&m_header, sizeof(m_header), 1, fp);//情報を書き込み

	fclose(fp);
}

void Score::PointInit()
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "wb");
	assert(fp);//ファイルを開けなかったら止める
	//初期化
	Header header[static_cast<int>(Rank::Max)];
	header[static_cast<int>(Rank::First)].rank = Rank::First;
	header[static_cast<int>(Rank::First)].point = 250;
	header[static_cast<int>(Rank::Second)].rank = Rank::Second;
	header[static_cast<int>(Rank::Second)].point = 200;
	header[static_cast<int>(Rank::Third)].rank = Rank::Third;
	header[static_cast<int>(Rank::Third)].point = 150;
	header[static_cast<int>(Rank::Fourth)].rank = Rank::Fourth;
	header[static_cast<int>(Rank::Fourth)].point = 100;
	header[static_cast<int>(Rank::Fifth)].rank = Rank::Fifth;
	header[static_cast<int>(Rank::Fifth)].point = 50;
	fwrite(&header, sizeof(header), 1, fp);
	fclose(fp);
}
