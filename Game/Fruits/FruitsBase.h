#pragma once
#include "../../Util/Geometry.h"
#include <memory>

class HpBar;

/// <summary>
/// フルーツ基底クラス
/// </summary>
class FruitsBase
{
public:
	FruitsBase(Position2 pos, int handle);
	virtual ~FruitsBase();

	virtual void Update();
	virtual void Draw() = 0;
	/// <summary>
	/// 矩形を取得
	/// </summary>
	/// <returns>矩形構造体</returns>
	const Rect& GetRect()const;

	void SetPos(const Position2& pos);

	/// <summary>
	/// 攻撃を受けた
	/// </summary>
	/// <param name="i">攻撃力</param>
	void OnDamage(int i);

	/// <summary>
	/// 現在のHPを取得する
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetHp();

	/// <summary>
	/// 当たり判定対象か
	/// </summary>
	/// <returns>true:当たる false:当たらない</returns>
	bool IsCollidable()const;

	/// <summary>
	/// 存在しているかどうか
	/// </summary>
	/// <returns>true:存在する false:存在しない</returns>
	bool IsExist()const { return m_isExist; }

	/// <summary>
	/// 存在しているかどうかを指定して決める
	/// </summary>
	/// <param name="exist">存在しているかどうか</param>
	void SetExist(bool exist) { m_isExist = exist; }

	/// <summary>
	/// 壊されたかどうか
	/// </summary>
	/// <returns>true:壊された false:ぶつかった</returns>
	bool IsDestroy() const { return m_isDestroy; }

	/// <summary>
	/// 壊した
	/// </summary>
	void SetDestroy() { m_isDestroy = true; }

	/// <summary>
	/// フルーツの得点を取得する
	/// </summary>
	/// <returns>得点</returns>
	int GetPoint()const { return m_point; }

	virtual std::shared_ptr<FruitsBase> Clone() = 0;
protected:
	int m_handle;//画像

	int m_blend;//ブレンド値

	float m_scale;//画像の拡大率

	static constexpr int kSizeW = 32;//サイズX
	static constexpr int kSizeH = 32;//サイズY
	Rect m_rect;//矩形

	Vector2 m_vec;//移動

	bool m_isExist ;//存在しているかどうか

	int m_flashingTimer ;//点滅時間

	static constexpr int kAnimNum = 17;
	static constexpr int kAnimSpeed = 5;
	int m_idxX ;//画像インデックス

	float m_upperPower;//上に上がる
	float m_speed;//落ちていく速度

	std::shared_ptr<HpBar> m_hp;//フルーツのHp

	bool m_isDestroy;//破壊されたか

	int m_point;//得点
};

