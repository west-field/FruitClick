#pragma once
#include <vector>
#include "../Util/Geometry.h"

using MapData_t = std::vector<std::vector<unsigned char>>;

class Stage
{
public:
	Stage();
	virtual ~Stage();

	/// <summary>
	/// ヘッダーを読み込む
	/// </summary>
	/// <param name="filePath">ファイル名</param>
	void Load(const wchar_t* filePath);

	/// <summary>
	/// マップデータを取得する
	/// </summary>
	/// <returns></returns>
	const MapData_t& GetMapData()const;

	/// <summary>
	/// マップのIDを取得する関数
	/// </summary>
	/// <param name="layerId">レイヤー</param>
	/// <param name="chipX">チップの場所横</param>
	/// <param name="chipY">チップの場所縦</param>
	/// <returns>マップのID</returns>
	const int GetChipId(int layerId, int chipX, int chipY)const;

	/// <summary>
	/// マップのサイズを入手する
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void GetMapSize(int& width, int& height);

	/// <summary>
	/// スポナー数を返す
	/// </summary>
	/// <returns>スポナー数</returns>
	int GetMapSpawnerNum();

	/// <summary>
	/// マップの位置を取得する
	/// </summary>
	/// <param name="pos"></param>
	void GetMapPos(Position2& pos, int layerId, float posX, float posY);

	void Draw();
private:
	MapData_t m_mapData;	//マップデータ
	int m_mapWidth;			//マップの横幅
	int m_mapHeight;		//マップの縦幅

	int m_spawnerNum;//スポナー数
};

