#pragma once
#include "FruitsBase.h"

/// <summary>
/// ƒIƒŒƒ“ƒW
/// ‚­‚é‚­‚é‰ñ‚è‚È‚ª‚ç—Ž‚¿‚é
/// </summary>
class FruitsOrange : public FruitsBase
{
public:
	FruitsOrange(Position2 pos, int handle);
	virtual ~FruitsOrange();

	void Update() override;
	void Draw() override;

	virtual std::shared_ptr<FruitsBase> Clone() override;
private:
	float m_angle;//Šp“x
	int m_frame;
};

