#pragma once
#include "FruitsBase.h"

/// <summary>
/// �I�����W
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

