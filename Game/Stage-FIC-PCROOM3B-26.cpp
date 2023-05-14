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
	//FMF�w�b�_�[�iPlatinum�̃h�L�������g�ɏ����Ă���j
	struct Header
	{
		int8_t id[4];			//�@�t�@�C�����ʎq(FMF_)�@4�o�C�g
		uint32_t size;			//�@�w�b�_���̂������f�[�^�T�C�Y�@4�o�C�g
		uint32_t mapWidth;		//�@�}�b�v�̉����@4�o�C�g
		uint32_t mapHeight;		//�@�}�b�v�̏c���@4�o�C�g
		uint8_t chipWidth;		//�@�p�[�c(�Z��1��)�̉����@1�o�C�g
		uint8_t chipHeight;		//�@�p�[�c(�Z��1��)�̏c���@1�o�C�g
		uint8_t layerCount;		//�@���C���[���@1�o�C�g
		uint8_t bitCount;		//�@1�Z��������̃r�b�g��(��8�Ńo�C�g��)�@1�o�C�g
	};//20�o�C�g
	//�w�b�_�[�ɓǂݍ���
	Header header;
	//�t�@�C�����J��
	int handle = FileRead_open(filePath);
	//�S�Ă��擾����
	FileRead_read(&header, sizeof(header), handle);

	//fmf�t�@�C�����J�������ǂ���
	std::string strId;
	strId.resize(4);
	std::copy_n(header.id, 4, strId.begin());
	if (strId != "FMF_")
	{
		//���[�h���s
		assert(0);
	}

	//�}�b�v�̉��Əc���擾
	m_mapWidth = header.mapWidth;
	m_mapHeight = header.mapHeight;
	//���C���[��
	int layerCount = header.layerCount;
	//���C���[1������̃T�C�Y���v�Z����B�}�b�v�̕��~�}�b�v�̍����~�i�}�b�v�������̃o�C�g��
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	//��ڂ̃x�N�^�[�̃T�C�Y��ύX
	m_mapData.resize(layerCount);//���C���[�̐������擾�ł���悤��
	for (auto& layer : m_mapData)
	{
		layer.resize(layerDataSize);//���C���[�̃T�C�Y��
		FileRead_read(layer.data(), layerDataSize, handle);//���C���[�̃}�b�v��ǂݍ���
	}
	//�t�@�C�������
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

	//�}�b�v�`�b�v�̈ʒu
	pos  = { static_cast<float>(x * kBgSize),static_cast<float>(y * kBgSize) };
}
