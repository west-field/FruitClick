#pragma once
#include "FruitsBase.h"

/// <summary>
/// ÉCÉ`ÉS
/// </summary>
class FruitsStrawberry : public FruitsBase
{
public:
	FruitsStrawberry(Position2 pos, int handle);
	virtual ~FruitsStrawberry();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:

};

