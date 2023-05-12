#pragma once
#include "../Util/Geometry.h"
#include <memory>
#include "../Util/Info.h"

class HpBar;//HpBar�N���X

/// <summary>
/// �L�����N�^
/// </summary>
class Character
{
public:
	Character(int selectChar);
	virtual ~Character();

	//�X�V
	void Update();
	//�\��
	void Draw();

	void CharacterLoad(const wchar_t* charName);

	/// <summary>
	/// ��`���擾
	/// </summary>
	/// <returns>��`�\����</returns>
	const Rect& GetRect()const;

	/// <summary>
	/// �����蔻��Ώۂ�
	/// </summary>
	/// <returns>true:������ false:������Ȃ�</returns>
	bool IsCollidable()const;

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	void Damage(int damage);
	/// <summary>
	/// �񕜂���
	/// </summary>
	/// <param name="Heal">�񕜗�</param>
	void Heal(int Heal);

	int GetHp()const;

	/// <summary>
	/// ���݂��Ă��邩�ǂ���
	/// </summary>
	/// <returns>true:���݂��� false:���݂��Ȃ�</returns>
	bool IsExist()const { return m_isExist; }
private:
	//�L�����N�^�̃A�j���[�V����������
	int m_handle[static_cast<int>(CharAnimType::Max)] = {};

	//���Đ����Ă���A�A�j���[�V�����^�C�v
	CharAnimType m_animType;

	//���ɉ�������ł��邩(�A�j���[�V�����̎�ނ���
	int m_w[static_cast<int>(CharAnimType::Max)] = { 0 };

	Rect m_rect;

	int m_idxX = 0;//�\������摜�̈ʒuX
	int m_idxY = 0;//�\������摜�̈ʒuY

	bool m_isLeft = false;//�����ނ��Ă��邩�ǂ���

	float m_vecX = 0.0f;//���ړ�
	
	int m_frame = 0;//�A�j���[�V��������
	
	bool m_isExist = true;//���݂��Ă��邩

	int m_ultimateTimer = 0;//���G����

	std::shared_ptr<HpBar> m_hp;//HP
};
