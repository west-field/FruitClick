#pragma once
#include "../Util/Geometry.h"
#include <list>

/// <summary>
/// HP
/// </summary>
class HpBar
{
public:
	HpBar(Position2 pos);
	virtual ~HpBar();

	/// <summary>
	/// ������
	/// </summary>
	void Init();
	
	/// <summary>
	/// �ő�HP���w�肷��
	/// </summary>
	/// <param name="maxHp">�ő�HP</param>
	void MaxHp(int maxHp);

	/// <summary>
	/// �\������n�[�g�̃^�C�v��ݒ�
	/// </summary>
	void SetHeat();

	/// <summary>
	/// �X�V�v���C���[
	/// </summary>
	void UpdatePlayer();
	/// <summary>
	/// �\��
	/// </summary>
	void Draw();

	/// <summary>
	/// HP���擾����
	/// </summary>
	/// <returns>���݂�HP</returns>
	int GetHp() const { return m_Hp; }
	/// <summary>
	/// �ő�HP���擾����
	/// </summary>
	/// <returns>�ő�HP</returns>
	int GetMaxHp()const { return m_MaxHp; }
	/// <summary>
	/// �_���[�W�v�Z
	/// </summary>
	/// <param name="damage">�󂯂��_���[�W��</param>
	void Damage(int damage);
	/// <summary>
	/// �񕜌v�Z
	/// </summary>
	/// <param name="heal">�񕜗�</param>
	void Heal(const int heal);
private:
	/// <summary>
	/// HP���ő�l�܂���0�𒴂��Ă��Ȃ���
	/// </summary>
	/// <param name="hp">�v�Z����HP</param>
	virtual void SetHp(const int hp);

	int m_MaxHp;//�ő�HP
	int m_Hp;//���݂�HP
	int m_HpHandle;	//HP�̉摜
	int m_heatFull;//�n�[�g�t��
	int m_heatHalf;//�n�[�g�n�[�t

	enum class HeatType
	{
		Full,
		Half,
	};

	struct Heat
	{
		HeatType type;
		int posAddX;
		int posAddY;
		bool isExist;
	};

	std::list<Heat> m_heatHandle;

	Rect m_rect;

	int idxX = 0;
	int idxY = 0;
};

