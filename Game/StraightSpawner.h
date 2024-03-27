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

	/// <summary>
	/// �t���[�c����
	/// </summary>
	/// <returns>���������t���[�c</returns>
	virtual std::shared_ptr<FruitsBase> Spawn() override;
	virtual void Update() override;
private:
};

