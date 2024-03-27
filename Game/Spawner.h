#pragma once

#include <memory>
#include "../Util/Geometry.h"
#include "FruitsType.h"

class FruitsBase;
class FruitsFactory;

/// <summary>
/// �X�|�i�[���N���X
/// </summary>
class Spawner
{
public:
	Spawner(const Position2& pos,std::shared_ptr<FruitsBase> prototype,std::shared_ptr<FruitsFactory> factory);
	virtual ~Spawner();
	/// <summary>
	/// �t���[�c����
	/// </summary>
	/// <returns>��������t���[�c</returns>
	virtual std::shared_ptr<FruitsBase> Spawn() = 0;
	virtual void Update() = 0;
	/// <summary>
	/// �X�|�i�[�����邩�ǂ���
	/// </summary>
	/// <returns>true:���� false:�Ȃ�</returns>
	bool IsExist() const;
protected:
	Position2 m_pos;//�X�|�i�[���g�̍��W
	std::shared_ptr<FruitsBase> m_prototype;//�v���g�^�C�v(�N���[��)
	std::shared_ptr<FruitsFactory> m_factory;//�t�@�N�g���ւ̎Q��
	bool m_isExist = true;//�X�|�i�[���g�̎���
};

