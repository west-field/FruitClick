#include "Score.h"
#include <DxLib.h>
#include <cassert>
#include "../game.h"

namespace
{
	const char* path = "Data/score.info";

	static constexpr int pw_width = 400;
	static constexpr int pw_height = 300;
	static constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	static constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	static constexpr int kPosX = pw_start_x + 10;
	static constexpr int kPosY = pw_start_y + 10;

	static constexpr int kFontSize = 20;
}

Score::Score():m_point(), m_header()
{

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
	DrawString(pw_start_x + 10, pw_start_y + 10, L"ランキング", 0xffff88);//メッセージ
	int y = kFontSize * 2;
	for (auto& score : m_header)
	{
		DrawFormatString(kPosX, kPosY + y, 0xffffff, L"%d位：%d点", static_cast<int>(score.rank) + 1, score.point);
		y += kFontSize * 2;
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
	bool isChange = false;//情報を変更したかどうか

	//今回のポイントが一番下の取得したデータの得点よりも大きいとき
	if (score >= m_header[static_cast<int>(Rank::Max) - 1].point)
	{
		isChange = true;
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
	if (isChange)
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
