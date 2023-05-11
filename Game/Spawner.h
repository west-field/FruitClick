#pragma once

#include <memory>
#include "../Util/Geometry.h"
#include "FruitsType.h"

class FruitsBase;
class FruitsFactory;

/// <summary>
/// �\��������
/// </summary>
class Spawner
{
public:
	Spawner(const Position2& pos,std::shared_ptr<FruitsBase> prototype,std::shared_ptr<FruitsFactory> factory);
	virtual ~Spawner();
	virtual std::shared_ptr<FruitsBase> Spawn() = 0;//�t���[�c����
	virtual void Update() = 0;//�X�V
	bool IsExist() const;
protected:
	Position2 m_pos;//�X�|�i�[���g�̍��W
	std::shared_ptr<FruitsBase> m_prototype;//�v���g�^�C�v(�N���[��)
	std::shared_ptr<FruitsFactory> m_factory;//�t�@�N�g���ւ̎Q��
	bool m_isExist = true;//�X�|�i�[���g�̎���
};

