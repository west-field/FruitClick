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
	/// �w�b�_�[��ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C����</param>
	void Load(const wchar_t* filePath);

	/// <summary>
	/// �}�b�v�f�[�^���擾����
	/// </summary>
	/// <returns></returns>
	const MapData_t& GetMapData()const;

	/// <summary>
	/// �}�b�v��ID���擾����֐�
	/// </summary>
	/// <param name="layerId">���C���[</param>
	/// <param name="chipX">�`�b�v�̏ꏊ��</param>
	/// <param name="chipY">�`�b�v�̏ꏊ�c</param>
	/// <returns>�}�b�v��ID</returns>
	const int GetChipId(int layerId, int chipX, int chipY)const;

	/// <summary>
	/// �}�b�v�̃T�C�Y����肷��
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void GetMapSize(int& width, int& height);

	/// <summary>
	/// �X�|�i�[����Ԃ�
	/// </summary>
	/// <returns>�X�|�i�[��</returns>
	int GetMapSpawnerNum();

	/// <summary>
	/// �}�b�v�̈ʒu���擾����
	/// </summary>
	/// <param name="pos"></param>
	void GetMapPos(Position2& pos, int layerId, float posX, float posY);

	void Draw();
private:
	MapData_t m_mapData;	//�}�b�v�f�[�^
	int m_mapWidth;			//�}�b�v�̉���
	int m_mapHeight;		//�}�b�v�̏c��

	int m_spawnerNum;//�X�|�i�[��
};

