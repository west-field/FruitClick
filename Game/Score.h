#pragma once
#include <map>
#include <vector>
#include <string>

/// <summary>
/// 得点
/// </summary>
class Score
{
public:
	Score();
	virtual ~Score();
	void Draw();//得点を表示する

	/// <summary>
	/// 得点データを読み込む
	/// </summary>
	void Load();

	/// <summary>
	/// 比較する
	/// </summary>
	void Comparison(int score);

	bool IsSave()const { return m_isSave; }
private:
	/// <summary>
	/// ウィンドウを画像で表示する
	/// </summary>
	void WindowDraw();
	/// <summary>
	/// 得点データを書き込む
	/// </summary>
	void Save();

	/// <summary>
	/// ランキングを初期化する
	/// </summary>
	void PointInit();

	int m_point;//自分のポイント

	/// <summary>
	/// 順位
	/// </summary>
	enum class Rank
	{
		First,
		Second,
		Third,
		Fourth,
		Fifth,

		Max
	};

	/// <summary>
	/// ファイル保存用構造体
	/// </summary>
	struct Header
	{
		Rank rank;
		int point;
	};
	Header m_header[static_cast<int>(Rank::Max)];

	bool m_isSave = false;//情報を変更したかどうか

	int m_bg;
};

