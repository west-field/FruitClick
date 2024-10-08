#include "Mouse.h"
#include <DxLib.h>
#include "DrawFunctions.h"
#include "../game.h"

namespace
{
	constexpr int kRipplesImgWidth = 100;//波紋画像サイズX
	constexpr int kRipplesImgHeight = 100;//波紋画像サイズY
	constexpr float kRipplesDrawScale = 1.0f;//波紋拡大率
	constexpr int kRipplesAnimNum = 31;//波紋アニメーション枚数
	constexpr int  kRipplesAnimSpeed = 1;//波紋アニメーションスピード
}

Mouse::Mouse() :m_mouseLog(), m_rect{ {},{} }
{
	m_mouseH = my::MyLoadGraph(L"Data/Cursor.png");
	m_ripplesH = my::MyLoadGraph(L"Data/ripples.png");
	MouseReset();
}

Mouse::~Mouse()
{
	DeleteGraph(m_mouseH);
	DeleteGraph(m_ripplesH);
}

void Mouse::Update()
{
	//ログの更新
	for (int i = kLogNum - 1; i >= 1; i--)
	{
		m_mouseLog[i] = m_mouseLog[i - 1];
	}

	//左クリックしたとき
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		m_mouseLog[0] = InputType::left;
		if (IsTrigger(InputType::left))
		{
			ClickAnimCreate();
		}
	}
	//右クリックしたとき
	else if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		m_mouseLog[0] = InputType::right;
		if (IsTrigger(InputType::right))
		{
			ClickAnimCreate();
		}
	}
	//何も押していないとき
	else
	{
		m_mouseLog[0] = InputType::noPressed;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//波紋の更新
	RipplesAnimUpdate();

	//マウスカーソルの位置を取得
	m_rect.center = GetMousePos();
}

void Mouse::Draw()
{
	//波紋を表示する
	for (auto& ripples : m_ripples)
	{
		//波紋を表示できるとき
		if (ripples.isDisplay)
		{
			int animNum = static_cast<int>(ripples.idx / kRipplesAnimSpeed);
			if (animNum >= kRipplesAnimNum)
			{
				animNum -= kRipplesAnimNum;
			}
			int imgX = animNum % 6 * kRipplesImgWidth;
			int imgY = animNum / 6 * kRipplesImgHeight;
			my::MyDrawRectRotaGraph(ripples.pos.x, ripples.pos.y, imgX, imgY, kRipplesImgWidth, kRipplesImgHeight, kRipplesDrawScale, 0.0f, m_ripplesH, true, false);
		}
	}

	//マウスカーソルを表示する
	my::MyDrawGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), m_mouseH, true);

#ifdef _DEBUG
	//マウスカーソルの位置を表示
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0xffffff, L"-----x%3f,y%3f", m_rect.center.x, m_rect.center.y);
#endif
}

void Mouse::MouseReset()
{
	//マウスカーソルを中心に移動させる
	SetMousePoint(Game::kScreenWidth / 2, Game::kScreenHeight / 2);
}

Position2 Mouse::GetMousePos() const
{
	int mouseX = 0;//マウスカーソルx座標
	int mouseY = 0;//マウスカーソルy座標

	//マウスカーソルの位置を取得する関数の返り値が-1の時
	if (GetMousePoint(&mouseX, &mouseY) == -1)
	{
		//エラー発生
		return Position2(0, 0);
	}

	return Position2(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

bool Mouse::IsPress(InputType type) const
{
	//最新のログが同じ時、押されている
	return (m_mouseLog[0] == type);
}

bool Mouse::IsTrigger(InputType type) const
{
	bool isNow = (m_mouseLog[0] == type);//現在の状態
	bool isLast = (m_mouseLog[1] == InputType::noPressed);//1フレーム前の状態

	return (isNow && isLast);//今押していて、1フレーム前に何も押していないとき押せる判定
}

bool Mouse::IsRelase(InputType type) const
{
	bool isNow = (m_mouseLog[0] == InputType::noPressed);//現在の状態
	bool isLast = (m_mouseLog[1] == type);//1フレーム前の状態
	return (isNow && isLast);//今離していて、1フレーム前押されていたとき離した判定になる
}

Rect Mouse::GetRect() const
{
	//当たり判定を返す
	return m_rect;
}

bool Mouse::MouseSelect(int startX, int endX, int startY, int endY) const
{
	//カーソルの位置がメニュー位置にいるかどうか	
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//マウスの位置が範囲内にある
	return true;
}

bool Mouse::MouseSelect(float startX, float endX, float startY, float endY) const
{
	//カーソルの位置がメニュー位置にいるかどうか
	if (m_rect.center.y < startY)	return false;
	if (m_rect.center.y > endY)		return false;
	if (m_rect.center.x < startX)	return false;
	if (m_rect.center.x > endX)		return false;

	//マウスの位置が範囲内にある
	return true;
}

void Mouse::ClickAnimCreate()
{
	//波紋作成
	Ripples ripples;
	ripples.pos = m_rect.center;//現在の位置
	ripples.isDisplay = true;//波紋を表示する
	ripples.idx = 0;//一番最初の絵のインデックス

	//一番後ろに入れる
	m_ripples.push_back(ripples);
}

void Mouse::RipplesAnimUpdate()
{
	//波紋の更新
	for (auto& ripples : m_ripples)
	{
		//既に表示できなくなっている時は処理しない
		if (!ripples.isDisplay)	continue;

		//インデックスがアニメーション枚数以上のとき
		if (ripples.idx++ >= kRipplesAnimNum * kRipplesAnimSpeed)
		{
			ripples.idx = 0;
			ripples.isDisplay = false;
		}
	}

	//いらなくなったものを削除
	m_ripples.remove_if([](const Ripples click) {
		return !click.isDisplay;
		});
}

