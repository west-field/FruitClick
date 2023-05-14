#include "Stage.h"
#include <DxLib.h>
#include <string>
#include <cassert>
#include "../Util/DrawFunctions.h"

namespace
{
	constexpr int kBgSize = 64;
}

Stage::Stage(int handle) :m_mapWidth(0), m_mapHeight(0),m_handle(handle)
{
	
}

Stage::~Stage()
{
}

void Stage::Load(const wchar_t* filePath)
{
	//FMFヘッダー（Platinumのドキュメントに書いてある）
	struct Header
	{
		int8_t id[4];			//　ファイル識別子(FMF_)　4バイト
		uint32_t size;			//　ヘッダをのぞいたデータサイズ　4バイト
		uint32_t mapWidth;		//　マップの横幅　4バイト
		uint32_t mapHeight;		//　マップの縦幅　4バイト
		uint8_t chipWidth;		//　パーツ(セル1個)の横幅　1バイト
		uint8_t chipHeight;		//　パーツ(セル1個)の縦幅　1バイト
		uint8_t layerCount;		//　レイヤー数　1バイト
		uint8_t bitCount;		//　1セル当たりのビット数(÷8でバイト数)　1バイト
	};//20バイト
	//ヘッダーに読み込む
	Header header;
	//ファイルを開く
	int handle = FileRead_open(filePath);
	//全てを取得する
	FileRead_read(&header, sizeof(header), handle);

	//fmfファイルを開いたかどうか
	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());
	if (strId != "FMF_")
	{
		//ロード失敗
		assert(0);
	}

	//マップの横と縦を取得
	m_mapWidth = header.mapWidth;
	m_mapHeight = header.mapHeight;
	//レイヤー数
	int layerCount = header.layerCount;
	//レイヤー1個あたりのサイズを計算する。マップの幅×マップの高さ×（マップ一個当たりのバイト数
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	//一つ目のベクターのサイズを変更
	m_mapData.resize(layerCount);//レイヤーの数だけ取得できるように
	for (auto& layer : m_mapData)
	{
		layer.resize(layerDataSize);//レイヤーのサイズ分
		FileRead_read(layer.data(), layerDataSize, handle);//レイヤーのマップを読み込む
	}
	//ファイルを閉じる
	FileRead_close(handle);
}

const MapData_t& Stage::GetMapData() const
{
	return m_mapData;
}

void Stage::DrawMap()
{
	for (int y = 0; y < m_mapHeight; y++)
	{
		for (int x = 0; x < m_mapWidth; x++)
		{
			int X = x * kBgSize;
			int Y = y * kBgSize;
			my::MyDrawRectRotaGraph(X, Y, 0, 0, kBgSize, kBgSize, 1.0f, 0.0f, m_handle, true, false);
		}
	}
}

const int Stage::GetChipId(int layerId, int chipX, int chipY) const
{
	assert(chipX < m_mapWidth);
	assert(chipY < m_mapHeight);
	auto index = chipX + m_mapWidth * chipY;
	return m_mapData[layerId][index];
}

void Stage::GetMapSize(int& width, int& height)
{
	width = m_mapWidth;
	height = m_mapHeight;
}

void Stage::GetMapPos(Position2& pos, int layerId, float posX, float posY)
{
	int x, y;
	x = static_cast<int>(posX) / kBgSize;
	y = static_cast<int>(posY) / kBgSize;

	//マップチップの位置
	pos  = { static_cast<float>(x * kBgSize),static_cast<float>(y * kBgSize) };
}
