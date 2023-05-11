#pragma once
#include "FruitsBase.h"

/// <summary>
/// ÉLÉEÉC
/// </summary>
class FruitsKiwi : public FruitsBase
{
public:
	FruitsKiwi(Position2 pos, int handle);
	virtual ~FruitsKiwi();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:

};

