#pragma once

#include "Geometry.h"
#include <list>

class InputState;

class Mouse
{
public:
	Mouse();
	virtual ~Mouse();
	void Update(const InputState& input);
	void Draw();

	void MouseReset();

	//現在のマウス位置取得
	Position2 GetPos();

	//押し下げ判定
	bool IsPressLeft();
	//トリガー判定
	bool IsTriggerLeft();
	//話した判定
	bool IsRelaseLeft();

	//当たり矩形
	Rect& GetRect();

	/// <summary>
	/// メニューセレクト位置にいるかどうか
	/// </summary>
	/// <param name="startY">y軸の上座標</param>
	/// <param name="endY">y軸の下座標</param>
	/// <param name="startY">y軸の上座標</param>
	/// <param name="endY">y軸の下座標</param>
	/// <returns>true:押せる false:押せない</returns>
	bool MouseSelect(int startX, int endX,int startY, int endY);

	bool MouseSelect(float startX, float endX,float startY, float endY);

	/// <summary>
	/// クリックしたときに出す波紋を作成
	/// </summary>
	void ClickAnimCreate();

	/// <summary>
	/// クリックしたときに出す波紋の更新
	/// </summary>
	void ClickAnimUpdate();
private:
    Rect m_rect;//マウスの位置
    int m_mouseH;//マウスの画像

	struct Click
	{
		Position2 pos;
		bool isClick;
		int idx;
	};
	std::list<Click> m_click;
	int m_clickH;

	static constexpr int kLogNum = 16;
	//入力ログ　0が最新の状態
	int mouseLog[kLogNum];
};

