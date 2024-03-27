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

	/// <summary>
	/// フルーツ発生
	/// </summary>
	/// <returns>生成したフルーツ</returns>
	virtual std::shared_ptr<FruitsBase> Spawn() override;
	virtual void Update() override;
private:
};

