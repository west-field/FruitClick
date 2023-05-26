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

	/// <summary>
	/// 得点データを書き込む
	/// </summary>
	void Save();
private:
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
};

