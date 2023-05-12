#pragma once
#include "Spawner.h"

/// <summary>
/// �t���[�c���܂�����������
/// </summary>
class StraightSpawner : public Spawner
{
public:
	StraightSpawner(const Position2& pos, FruitsType type, std::shared_ptr<FruitsFactory> factory);
	virtual ~StraightSpawner();

	virtual std::shared_ptr<FruitsBase> Spawn() override;//�t���[�c����
	virtual void Update() override;//�X�V
private:
};
