#pragma once

#include "Geometry.h"
#include <list>

class InputState;

class Mouse	final
{
public:
	Mouse();
	virtual ~Mouse();
	void Update();
	void Draw();

	void MouseReset();

	/// <summary>
	/// 現在のマウス位置取得
	/// </summary>
	/// <returns></returns>
	Position2 GetPos() const;

	//押し下げ判定
	bool IsPressLeft();
	bool IsPressRight();
	//トリガー判定
	bool IsTriggerLeft();
	bool IsTriggerRight();
	//離した判定
	bool IsRelaseLeft();
	bool IsRelaseRight();

	//当たり矩形
	Rect GetRect() const;

	/// <summary>
	/// カーソルの位置がメニューセレクト位置にいるかどうか
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
	void RipplesAnimUpdate();
private:
    Rect m_rect;//マウスの位置
    int m_mouseH;//マウスの画像

	// 波紋を表示するために必要な構造体
	struct Ripples
	{
		Position2 pos;//位置
		bool isDisplay;//表示できるかどうか
		int idx;//画像のインデックス
	};
	
	std::list<Ripples> m_ripples;//波紋
	int m_ripplesH;//波紋画像

	static constexpr int kLogNum = 16;
	//入力ログ　0が最新の状態
	int mouseLog[kLogNum];
};

