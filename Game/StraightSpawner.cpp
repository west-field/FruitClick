#include "StraightSpawner.h"
#include "Fruits/FruitsBase.h"
#include "FruitsFactory.h"

StraightSpawner::StraightSpawner(const Position2& pos, FruitsType type, std::shared_ptr<FruitsFactory> factory):
	Spawner(pos,factory->Make(type),factory)
{
}

StraightSpawner::~StraightSpawner()
{
}

std::shared_ptr<FruitsBase> StraightSpawner::Spawn()
{
	m_isExist = false;//���������玀��
	m_prototype->SetPos(m_pos);//�t���[�c���g�̍��W��ݒ�
	m_factory->AddFruits(m_prototype);//�t���[�c���X�g�ɒǉ�
	
	return m_prototype;
}

void StraightSpawner::Update()
{
	Spawn();
}
