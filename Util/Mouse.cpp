#include "Mouse.h"
#include <DxLib.h>
#include "DrawFunctions.h"
#include "../InputState.h"
#include "../game.h"

namespace
{
	constexpr int kClickImgWidth = 100;//画像サイズX
	constexpr int kClickImgHeight = 100;//画像サイズY
	constexpr float kClickDrawScale = 1.0f;//拡大率
	constexpr int kClickAnimNum = 31;//アニメーション枚数
	constexpr int  kClickAnimSpeed = 1;//アニメーションスピード
}

Mouse::Mouse():mouseLog()
{
	m_rect = { {},{} };
	m_mouseH = my::MyLoadGraph(L"Data/Cursor.png");
	m_clickH = my::MyLoadGraph(L"Data/click.png");
	MouseReset();
}

Mouse::~Mouse()
{
	DeleteGraph(m_mouseH);
}

void Mouse::Update(const InputState& input)
{
	//ログの更新
	for (int i = kLogNum - 1; i >= 1; i--)
	{
		mouseLog[i] = mouseLog[i - 1];
	}

	//最新の状態
	if (input.IsTriggered(InputType::slect))
	{
		mouseLog[0] = 1;
		ClickAnimCreate();
	}
	else if (input.IsTriggered(InputType::prev))
	{
		mouseLog[0] = 1;
	}
	else
	{
		mouseLog[0] = 0;
	}

	ClickAnimUpdate();


	m_rect.center = GetPos();
}

void Mouse::Draw()
{
	for (auto& click : m_click)
	{
		if (click.isClick)
		{
			int animNum = static_cast<int>(click.idx / kClickAnimSpeed);
			if (animNum >= kClickAnimNum)
			{
				animNum -= kClickAnimNum;
			}
			int imgX = animNum % 6 * kClickImgWidth;
			int imgY = animNum / 6 * kClickImgHeight;
			my::MyDrawRectRotaGraph(click.pos.x, click.pos.y, imgX, imgY, kClickImgWidth, kClickImgHeight, kClickDrawScale, 0.0f, m_clickH, true, false);
		}
	}
	my::MyDrawGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), m_mouseH, true);
#ifdef _DEBUG
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0xffffff, L"-----x%3f,y%3f", m_rect.center.x, m_rect.center.y);
#endif
}

void Mouse::MouseReset()
{
	SetMousePoint(Game::kScreenWidth / 2, Game::kScreenHeight / 2);//ポインタを中心に移動させる
}

Position2 Mouse::GetPos()
{
	Position2 mousePos{ 0,0 };
	int mouseX = 0;
	int mouseY = 0;
	if (GetMousePoint(&mouseX, &mouseY) == -1)
	{
		//エラー発生
		return mousePos;
	}
	mousePos.x = static_cast<float>(mouseX);
	mousePos.y = static_cast<float>(mouseY);

	return mousePos;
}

bool Mouse::IsPressLeft()
{
	//最新のログが1の時押されている
	return (mouseLog[0]);
}

bool Mouse::IsTriggerLeft()
{
	bool isNow = (mouseLog[0]);//現在の状態
	bool isLast = (mouseLog[1]);//1フレーム前の状態
	return (isNow && !isLast);//今押していて、1フレーム前押されていなかったとき押せる
}

bool Mouse::IsRelaseLeft()
{
	bool isNow = (mouseLog[0]);//現在の状態
	bool isLast = (mouseLog[1]);//1フレーム前の状態
	return (!isNow && isLast);//今離していて、1フレーム前押されていたとき離した判定になる
}

Rect& Mouse::GetRect()
{
	return m_rect;
}

bool Mouse::MouseSelect(int startX, int endX, int startY, int endY)
{
	
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//マウスの位置が範囲内にある
	return true;
}

bool Mouse::MouseSelect(float startX, float endX, float startY, float endY)
{
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//マウスの位置が範囲内にある
	return true;
}

void Mouse::ClickAnimCreate()
{
	Click click;
	click.pos = m_rect.center;
	click.isClick = true;
	click.idx = 0;

	m_click.push_back(click);
}

void Mouse::ClickAnimUpdate()
{
	for (auto& click : m_click)
	{
		if (!click.isClick)	continue;

		if (click.idx++ >= kClickAnimNum * kClickAnimSpeed)
		{
			click.idx = 0;
			click.isClick = false;
		}
	}

	//いらなくなったものを削除
	m_click.remove_if([](const Click click) {
		return !click.isClick;
		});
}

