#pragma once
#include "FruitsBase.h"

/// <summary>
/// �I�����W
/// ���邭����Ȃ��痎����
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
	float m_angle;//�p�x
	int m_frame;
};

