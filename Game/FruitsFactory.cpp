#include "FruitsFactory.h"
#include <DxLib.h>

#include "Fruits/FruitsBase.h"
#include "Fruits/FruitsApple.h"
#include "Fruits/FruitsBanana.h"
#include "Fruits/FruitsCherrie.h"
#include "Fruits/FruitsKiwi.h"
#include "Fruits/FruitsMelon.h"
#include "Fruits/FruitsOrange.h"
#include "Fruits/FruitsPineapple.h"
#include "Fruits/FruitsStrawberry.h"

#include "../Util/DrawFunctions.h"

FruitsFactory::FruitsFactory() : m_count(0)
{
	m_handleMap[FruitsType::Apple] = my::MyLoadGraph(L"Data/Fruits/Apple.png");
	m_handleMap[FruitsType::Banana] = my::MyLoadGraph(L"Data/Fruits/Bananas.png");
	m_handleMap[FruitsType::Cherrie] = my::MyLoadGraph(L"Data/Fruits/Cherries.png");
	m_handleMap[FruitsType::Kiwi] = my::MyLoadGraph(L"Data/Fruits/Kiwi.png");
	m_handleMap[FruitsType::Melon] = my::MyLoadGraph(L"Data/Fruits/Melon.png");
	m_handleMap[FruitsType::Orange] = my::MyLoadGraph(L"Data/Fruits/Orange.png");
	m_handleMap[FruitsType::Pineapple] = my::MyLoadGraph(L"Data/Fruits/Pineapple.png");
	m_handleMap[FruitsType::Strawberry] = my::MyLoadGraph(L"Data/Fruits/Strawberry.png");
}

FruitsFactory::~FruitsFactory()
{
	for (auto& handle : m_handleMap)
	{
		DeleteGraph(handle.second);
	}
}

void FruitsFactory::Update()
{
	for (auto& fruit : m_fruits)
	{
		if (fruit->IsDestroy())
		{
			m_count++;
		}
	}

	//Ç¢Ç»Ç≠Ç»Ç¡ÇΩìGÇÕè¡Ç¶ÇÈ
	auto rmIt = std::remove_if(m_fruits.begin(), m_fruits.end(),
		[](const std::shared_ptr<FruitsBase>& fruit) {
			return !fruit->IsExist();
		});
	m_fruits.erase(rmIt, m_fruits.end());

	for (auto& fruit : m_fruits)
	{
		if (fruit->IsExist())
		{
			fruit->Update();
		}
	}
}

void FruitsFactory::Draw()
{

	for (auto& fruit : m_fruits)
	{
		if (fruit->IsExist())
		{
			fruit->Draw();
		}
	}
#ifdef _DEBUG
	int num = 0;
	for (auto& fruit : m_fruits)
	{
		if (fruit->IsExist())
		{
			num++;
		}
	}
	DrawFormatString(100, 100, 0xffffff, L"fruits%d", num);
#endif
}

std::shared_ptr<FruitsBase> FruitsFactory::Make(FruitsType type)
{
	switch (type)
	{
	case FruitsType::Apple:
		return std::make_shared<FruitsApple>(Position2(),
			m_handleMap[FruitsType::Apple]);
	case FruitsType::Banana:
		return std::make_shared<FruitsBanana>(Position2(),
			m_handleMap[FruitsType::Banana]);
	case FruitsType::Cherrie:
		return std::make_shared<FruitsCherrie>(Position2(),
			m_handleMap[FruitsType::Cherrie]);
	case FruitsType::Kiwi:
		return std::make_shared<FruitsKiwi>(Position2(),
			m_handleMap[FruitsType::Kiwi]);
	case FruitsType::Melon:
		return std::make_shared<FruitsMelon>(Position2(),
			m_handleMap[FruitsType::Melon]);
	case FruitsType::Orange:
		return	std::make_shared<FruitsOrange>(Position2(),
			m_handleMap[FruitsType::Orange]);
	case FruitsType::Pineapple:
		return std::make_shared<FruitsPineapple>(Position2(),
			m_handleMap[FruitsType::Pineapple]);
	case FruitsType::Strawberry:
		return std::make_shared<FruitsStrawberry>(Position2(),
			m_handleMap[FruitsType::Strawberry]);
	}

	return nullptr;
}

void FruitsFactory::AddFruits(std::shared_ptr<FruitsBase> enemy)
{
	m_fruits.push_back(enemy);
}

std::list<std::shared_ptr<FruitsBase>>& FruitsFactory::GetFruits()
{
	return m_fruits;
}
