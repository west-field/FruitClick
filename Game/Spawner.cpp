#include "Spawner.h"

Spawner::Spawner(const Position2& pos, std::shared_ptr<FruitsBase> prototype, std::shared_ptr<FruitsFactory> factory):
	m_pos(pos),m_prototype(prototype),m_factory(factory)
{
}

Spawner::~Spawner()
{
}

bool Spawner::IsExist() const
{
	return m_isExist;
}
