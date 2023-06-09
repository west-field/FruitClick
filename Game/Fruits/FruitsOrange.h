#pragma once
#include "FruitsBase.h"

/// <summary>
/// オレンジ
/// くるくる回りながら落ちる
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
	float m_angle;//角度
	int m_frame;
};

