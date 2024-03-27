#pragma once

#include <memory>
#include "../Util/Geometry.h"
#include "FruitsType.h"

class FruitsBase;
class FruitsFactory;

/// <summary>
/// スポナー基底クラス
/// </summary>
class Spawner
{
public:
	Spawner(const Position2& pos,std::shared_ptr<FruitsBase> prototype,std::shared_ptr<FruitsFactory> factory);
	virtual ~Spawner();
	/// <summary>
	/// フルーツ発生
	/// </summary>
	/// <returns>生成するフルーツ</returns>
	virtual std::shared_ptr<FruitsBase> Spawn() = 0;
	virtual void Update() = 0;
	/// <summary>
	/// スポナーがあるかどうか
	/// </summary>
	/// <returns>true:ある false:ない</returns>
	bool IsExist() const;
protected:
	Position2 m_pos;//スポナー自身の座標
	std::shared_ptr<FruitsBase> m_prototype;//プロトタイプ(クローン)
	std::shared_ptr<FruitsFactory> m_factory;//ファクトリへの参照
	bool m_isExist = true;//スポナー自身の寿命
};

