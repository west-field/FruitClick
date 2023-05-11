#pragma once
#include "FruitsBase.h"

/// <summary>
/// �X�C�J
/// </summary>
class FruitsMelon : public FruitsBase
{
public:
	FruitsMelon(Position2 pos, int handle);
	virtual ~FruitsMelon();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:

};

