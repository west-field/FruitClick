#pragma once
#include "FruitsBase.h"

/// <summary>
/// ƒIƒŒƒ“ƒW
/// </summary>
class FruitsOrange : public FruitsBase
{
public:
	FruitsOrange(Position2 pos, int handle);
	virtual ~FruitsOrange();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:
};

