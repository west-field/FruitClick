#pragma once
#include "FruitsBase.h"

/// <summary>
/// �p�C�i�b�v��
/// </summary>
class FruitsPineapple : public FruitsBase
{
public:
	FruitsPineapple(Position2 pos, int handle);
	virtual ~FruitsPineapple();

	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:

};

