#pragma once

#include "Geometry.h"
#include <list>

class InputState;

/// <summary>
/// マウスクラス
/// クリック判定、クリックしたとき波紋を表示
/// </summary>
class Mouse	final
{
public :
	//入力する状態
	enum class InputType
	{
		noPressed,//押されていない
		left,//左クリック
		right,//右クリック
	};
public:
	Mouse();
	virtual ~Mouse();
	void Update();
	void Draw();

	//マウスカーソルを中心に移動させる
	void MouseReset();

	/// <summary>
	/// 現在のマウス位置取得
	/// </summary>
	/// <returns></returns>
	Position2 GetMousePos() const;

	/// <summary>
	/// 押し下げ判定
	/// </summary>
	/// <returns>true:押されている false:押されていない</returns>
	bool IsPress(InputType type) const;

	/// <summary>
	/// トリガー判定
	/// </summary>
	/// <returns>true:押した false:押していない</returns>
	bool IsTrigger(InputType type) const;

	/// <summary>
	/// 離した判定
	/// </summary>
	/// <returns>true:離した false:離していない</returns>
	bool IsRelase(InputType type) const;

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
	bool MouseSelect(int startX, int endX,int startY, int endY) const;
	bool MouseSelect(float startX, float endX,float startY, float endY) const;

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
	//入力ログ　0が最新の状態　1が左クリック　2が右クリック
	InputType mouseLog[kLogNum];
};

