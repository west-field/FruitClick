#pragma once
#include "FruitsBase.h"

/// <summary>
/// サクランボ
/// </summary>
class FruitsCherrie : public FruitsBase
{
public:
	FruitsCherrie(Position2 pos, int handle);
	virtual ~FruitsCherrie();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:

};

