#pragma once

#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

#include "FruitsType.h"

class FruitsBase;

class FruitsFactory
{
public:
	FruitsFactory();
	virtual ~FruitsFactory();

	void Update();
	void Draw();
	
	/// <summary>
	/// 敵オブジェクトの生成のみ行う　※内部のリストへは追加しない
	/// </summary>
	/// <param name="type">敵種別</param>
	/// <returns>敵オブジェクトを返す</returns>
	std::shared_ptr<FruitsBase> Make(FruitsType type);

	/// <summary>
	/// 引数として渡されたオブジェクトを敵リストに追加する
	/// </summary>
	/// <param name="enemy">敵オブジェクト</param>
	void AddFruits(std::shared_ptr<FruitsBase> enemy);

	/// <summary>
	/// エネミーをすべて取得
	/// </summary>
	/// <returns>エネミーすべて</returns>
	std::list<std::shared_ptr<FruitsBase>>& GetFruits();

	/// <summary>
	/// フルーツをクリックで壊した数を取得する
	/// </summary>
	/// <returns>フルーツを壊した数</returns>
	int GetCount()const { return m_count; }
private:
	std::list<std::shared_ptr<FruitsBase>> m_fruits;	//フルーツ

	std::map<FruitsType, int>m_handleMap;//フルーツの画像ハンドル

	int m_count;//フルーツを壊した数
};

