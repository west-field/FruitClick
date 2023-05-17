#pragma once

#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

#include "FruitsType.h"

class FruitsBase;

class FruitsFactory
{
public:
	FruitsFactory();
	virtual ~FruitsFactory();

	void Update();
	void Draw();
	
	/// <summary>
	/// �G�I�u�W�F�N�g�̐����̂ݍs���@�������̃��X�g�ւ͒ǉ����Ȃ�
	/// </summary>
	/// <param name="type">�G���</param>
	/// <returns>�G�I�u�W�F�N�g��Ԃ�</returns>
	std::shared_ptr<FruitsBase> Make(FruitsType type);

	/// <summary>
	/// �����Ƃ��ēn���ꂽ�I�u�W�F�N�g��G���X�g�ɒǉ�����
	/// </summary>
	/// <param name="enemy">�G�I�u�W�F�N�g</param>
	void AddFruits(std::shared_ptr<FruitsBase> enemy);

	/// <summary>
	/// �G�l�~�[�����ׂĎ擾
	/// </summary>
	/// <returns>�G�l�~�[���ׂ�</returns>
	std::list<std::shared_ptr<FruitsBase>>& GetFruits();

	/// <summary>
	/// �t���[�c���N���b�N�ŉ󂵂������擾����
	/// </summary>
	/// <returns>�t���[�c���󂵂���</returns>
	int GetCount()const { return m_count; }
private:
	std::list<std::shared_ptr<FruitsBase>> m_fruits;	//�t���[�c

	std::map<FruitsType, int>m_handleMap;//�t���[�c�̉摜�n���h��

	int m_count;//�t���[�c���󂵂���
};

