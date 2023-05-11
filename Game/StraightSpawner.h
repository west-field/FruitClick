#pragma once
#include "Spawner.h"

/// <summary>
/// フルーツがまっすぐ落ちる
/// </summary>
class StraightSpawner : public Spawner
{
public:
	StraightSpawner(const Position2& pos, FruitsType type, std::shared_ptr<FruitsFactory> factory);
	virtual ~StraightSpawner();

	virtual std::shared_ptr<FruitsBase> Spawn() override;//フルーツ発生
	virtual void Update() override;//更新
private:
};

