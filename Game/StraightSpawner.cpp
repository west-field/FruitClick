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
	m_isExist = false;//生成したら死ぬ
	m_prototype->SetPos(m_pos);//フルーツ自身の座標を設定
	m_factory->AddFruits(m_prototype);//フルーツリストに追加
	
	return m_prototype;
}

void StraightSpawner::Update()
{
	Spawn();
}
