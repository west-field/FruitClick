#pragma once
#include "../Util/Geometry.h"
#include <memory>
#include "../Util/Info.h"

class HpBar;//HpBarクラス

/// <summary>
/// キャラクタ
/// </summary>
class Character
{
public:
	Character(int selectChar,Position2 pos);
	virtual ~Character();

	//更新
	void Update();
	void Update(bool isPlay);
	//表示
	void Draw();
	//キャラクターロード
	void CharacterLoad(const wchar_t* charName);

	/// <summary>
	/// 矩形を取得
	/// </summary>
	/// <returns>矩形構造体</returns>
	const Rect& GetRect()const;

	/// <summary>
	/// 当たり判定対象か
	/// </summary>
	/// <returns>true:当たる false:当たらない</returns>
	bool IsCollidable()const;

	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void Damage(int damage);
	/// <summary>
	/// 回復する
	/// </summary>
	/// <param name="Heal">回復量</param>
	void Heal(int Heal);
	/// <summary>
	/// 現在のHPを取得する
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetHp()const;

	/// <summary>
	/// 存在しているかどうか
	/// </summary>
	/// <returns>true:存在する false:存在しない</returns>
	bool IsExist()const { return m_isExist; }

	/// <summary>
	/// 現在再生しているアニメーションタイプを取得する
	/// </summary>
	/// <returns>アニメーションタイプ</returns>
	CharAnimType GetAnimType()const;
	/// <summary>
	/// 再生したいアニメーションタイプを指定する
	/// </summary>
	/// <param name="type">アニメーションタイプ</param>
	void SetAnimType(CharAnimType type);

	int GetIdx()const;
	int GetAnimNum(CharAnimType type)const;
private:
	//キャラクタのアニメーション分だけ
	int m_handle[static_cast<int>(CharAnimType::Max)] = {};

	//今再生している、アニメーションタイプ
	CharAnimType m_animType;

	//横に何枚並んでいるか(アニメーションの種類だけ
	int m_w[static_cast<int>(CharAnimType::Max)] = { 0 };

	Rect m_rect;

	int m_idxX = 0;//表示する画像の位置X
	int m_idxY = 0;//表示する画像の位置Y

	bool m_isLeft = false;//左をむいているかどうか

	float m_vecX = 0.0f;//横移動
	
	int m_frame = 0;//アニメーション時間
	
	bool m_isExist = true;//存在しているか

	int m_ultimateTimer = 0;//無敵時間

	std::shared_ptr<HpBar> m_hp;//HP
};

