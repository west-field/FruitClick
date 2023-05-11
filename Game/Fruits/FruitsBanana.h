#pragma once
#include "FruitsBase.h"

class FruitsBanana : public FruitsBase
{
public:
	FruitsBanana(Position2 pos, int handle);
	virtual ~FruitsBanana();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:

};

