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
	/// 得点データを書き込む
	/// </summary>
	void Save();

	/// <summary>
	/// ランキングを初期化する
	/// </summary>
	void PointInit();

	int m_point;//自分のポイント

	enum class Rank
	{
		First,
		Second,
		Third,
		Fourth,
		Fifth,

		Max
	};

	struct Info
	{
		int point;//得点
	};

	struct Header
	{
		Rank rank;
		int point;
	};
	Header m_header[static_cast<int>(Rank::Max)];

	bool m_isSave = false;//情報を変更したかどうか
};

