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

	/// <summary>
	/// �t���[�c�̓��_���擾����
	/// </summary>
	/// <returns>���_</returns>
	int GetPoint()const { return m_point; }

	virtual std::shared_ptr<FruitsBase> Clone() = 0;
protected:
	int m_handle;//�摜

	int m_blend;//�u�����h�l

	float m_scale;//�摜�̊g�嗦

	static constexpr int kSizeW = 32;//�T�C�YX
	static constexpr int kSizeH = 32;//�T�C�YY
	Rect m_rect;//��`

	Vector2 m_vec;//�ړ�

	bool m_isExist ;//���݂��Ă��邩�ǂ���

	int m_flashingTimer ;//�_�Ŏ���

	static constexpr int kAnimNum = 17;
	static constexpr int kAnimSpeed = 5;
	int m_idxX ;//�摜�C���f�b�N�X

	float m_upperPower;//��ɏオ��
	float m_speed;//�����Ă������x

	std::shared_ptr<HpBar> m_hp;//�t���[�c��Hp

	bool m_isDestroy;//�j�󂳂ꂽ��

	int m_point;//���_
};

