#include "Score.h"
#include <DxLib.h>
#include <cassert>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	const char* path = "Data/score.info";

	constexpr int kFontSize = 20;

	constexpr int kAdd = 5;
	constexpr int kWindowWidth = 400;
	constexpr int kWindowHeight = 300 + kAdd * 7;
	constexpr int kWindowStartX = (Game::kScreenWidth - kWindowWidth) / 2;//左
	constexpr int kWindowStartY = kFontSize;//上

	constexpr int kPosX = kWindowStartX + 10;
	constexpr int kPosY = kWindowStartY + 10;
}

Score::Score():m_point(), m_header(), m_isSave(false)
{
	//PointInit();//ランキング初期化
	m_bg = my::MyLoadGraph(L"Data/panel_Example1.png");
}

Score::~Score()
{
	DeleteGraph(m_bg);
}

void Score::Draw()
{
	WindowDraw();
	
	SetFontSize(kFontSize * 2);
	int X = kPosX + ((kFontSize * 2) * 2);
	DrawString(X, kPosY, L"ランキング", 0x191970);//メッセージ
	int y = kFontSize * 2 + 10;
	int color = 0x696969;
	for (auto& score : m_header)
	{
		if (m_isSave && score.point == m_point)
		{
			color = 0xdaa520;
		}
		else if (color != 0x696969)
		{
			color = 0x696969;
		}
		DrawFormatString(X, kPosY + y, color, L"%d位：%d点", static_cast<int>(score.rank) + 1, score.point);
		y += kFontSize * 2 + kAdd;
	}
	if (!m_isSave)
	{
		DrawFormatString(kPosX+ (kFontSize), kPosY + y, 0xdaa520, L"ランキング外:%d点", m_point);
	}
	SetFontSize(0);
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

void Score::WindowDraw()
{
	//角表示
	int x = kWindowStartX - 50 / 2;
	int y = kWindowStartY - 50 / 2;
	DrawRectGraph(x, y,
		0, 0, 50, 50, m_bg, true);//左上　50 y3,x9
	DrawRectGraph(x + kWindowWidth, y,
		50 * 8, 0, 50, 50, m_bg, true);//右上
	DrawRectGraph(x, y + kWindowHeight,
		0, 50 * 2, 50, 50, m_bg, true);//左下　50 y3,x9
	DrawRectGraph(x + kWindowWidth, y + kWindowHeight,
		50 * 8, 50 * 2, 50, 50, m_bg, true);//右下

	//画像の左上、右下、グラフィックの左上からXサイズ、Yサイズ、表示する画像、透明
	DrawRectExtendGraph(x + 50, y, x + kWindowWidth, y + 50,
		50 * 2, 0, 50, 50, m_bg, true);//上
	DrawRectExtendGraph(x, y + 50, x + 50, y + kWindowHeight,
		0, 50 * 1, 50, 50, m_bg, true);//左
	DrawRectExtendGraph(x + kWindowWidth, y + 50, x + kWindowWidth + 50, y + kWindowHeight,
		50 * 8, 50, 50, 50, m_bg, true);// 右
	DrawRectExtendGraph(x + 50, y + kWindowHeight, x + kWindowWidth, y + kWindowHeight + 50,
		50 * 2, 50 * 2, 50, 50, m_bg, true);	// 下
	DrawRectExtendGraph(x + 50, y + 50, x + kWindowWidth, y + kWindowHeight,
		50 * 3, 50 * 1, 50, 50, m_bg, true);	// ウインドウ内部
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
