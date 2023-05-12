#pragma once
#include "../../Util/Geometry.h"
#include <memory>

class HpBar;

/// <summary>
/// �t���[�c���N���X
/// </summary>
class FruitsBase
{
public:
	FruitsBase(Position2 pos, int handle);
	virtual ~FruitsBase();

	virtual void Update();
	virtual void Draw() = 0;
	/// <summary>
	/// ��`���擾
	/// </summary>
	/// <returns>��`�\����</returns>
	const Rect& GetRect()const;

	void SetPos(const Position2& pos);

	/// <summary>
	/// �U�����󂯂�
	/// </summary>
	/// <param name="i">�U����</param>
	void OnDamage(int i);

	/// <summary>
	/// ���݂�HP���擾����
	/// </summary>
	/// <returns>���݂�HP</returns>
	int GetHp();

	/// <summary>
	/// �����蔻��Ώۂ�
	/// </summary>
	/// <returns>true:������ false:������Ȃ�</returns>
	bool IsCollidable()const;

	/// <summary>
	/// ���݂��Ă��邩�ǂ���
	/// </summary>
	/// <returns>true:���݂��� false:���݂��Ȃ�</returns>
	bool IsExist()const { return m_isExist; }

	/// <summary>
	/// ���݂��Ă��邩�ǂ������w�肵�Č��߂�
	/// </summary>
	/// <param name="exist">���݂��Ă��邩�ǂ���</param>
	void SetExist(bool exist) { m_isExist = exist; }

	/// <summary>
	/// �󂳂ꂽ���ǂ���
	/// </summary>
	/// <returns>true:�󂳂ꂽ false:�Ԃ�����</returns>
	bool IsDestroy() const { return m_isDestroy; }

	/// <summary>
	/// �󂵂�
	/// </summary>
	void SetDestroy() { m_isDestroy = true; }

	virtual std::shared_ptr<FruitsBase> Clone() = 0;
protected:
	int m_handle;//�摜

	int m_blend;//�u�����h�l

	float m_scale;//�摜�̊g�嗦

	Rect m_rect;//��`

	Vector2 m_vec;//�ړ�

	bool m_isExist ;//���݂��Ă��邩�ǂ���

	int m_flashingTimer ;//�_�Ŏ���

	int m_idxX ;
	int m_idxY ;

	float m_jump;
	float m_speed;

	std::shared_ptr<HpBar> m_hp;

	bool m_isDestroy;//�j�󂳂ꂽ��
};
