#pragma once
#include "FruitsBase.h"

/// <summary>
/// �����S
/// </summary>
class FruitsApple : public FruitsBase
{
public:
	FruitsApple(Position2 pos, int handle);
	virtual ~FruitsApple();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:
};

