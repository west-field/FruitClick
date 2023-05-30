#pragma once
#include <map>
#include <vector>
#include <string>

/// <summary>
/// ���_
/// </summary>
class Score
{
public:
	Score();
	virtual ~Score();
	void Draw();//���_��\������

	/// <summary>
	/// ���_�f�[�^��ǂݍ���
	/// </summary>
	void Load();

	/// <summary>
	/// ��r����
	/// </summary>
	void Comparison(int score);

	bool IsSave()const { return m_isSave; }
private:
	/// <summary>
	/// ���_�f�[�^����������
	/// </summary>
	void Save();

	/// <summary>
	/// �����L���O������������
	/// </summary>
	void PointInit();

	int m_point;//�����̃|�C���g

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
		int point;//���_
	};

	struct Header
	{
		Rank rank;
		int point;
	};
	Header m_header[static_cast<int>(Rank::Max)];

	bool m_isSave = false;//����ύX�������ǂ���
};

