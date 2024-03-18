#pragma once
#include <unordered_map>

//�Q�[�����g�p���鉹
enum class SoundId
{
	Cursor,			//�J�[�\���ړ���
	Determinant,	//����{�^��
	Back,			//�߂�{�^��
	
	Count,			//�J�E���g�_�E����
	Start,			//�X�^�[�g��
	Point,			//���_�擾��
	
	PlayerMove,		//�v���C���[�ړ���
	Hit,			//�ڐG��
	
	FruitCreate,	//�t���[�c������
	FruitClick,		//�t���[�c�N���b�N��
	FruitDelete,	//�t���[�c�j��

	SoundId_Max
};

//�T�E���h�}�l�[�W���[
class SoundManager
{
private:
	SoundManager();
	
	//�R�s�[��������֎~����
	SoundManager(const SoundManager&) = delete;
	void operator= (const SoundManager&) = delete;

	/// <summary>
	/// �T�E���h�n���h�����擾
	/// </summary>
	/// <param name="id">�T�E���h��</param>
	/// <param name="fileName">�T�E���h�t�@�C����</param>
	/// <returns>�T�E���h�n���h��</returns>
	int LoadSoundFile(SoundId id,const wchar_t* fileName);
	
	/// <summary>
	/// �T�E���h���̃��[�h
	/// </summary>
	void LoadSoundConfig();

	std::unordered_map<SoundId, int> m_nameAndHandleTable;//�T�E���h�n���h��

	//�ύX�����T�E���h�����t�@�C���ɏ�������
	struct SoundConfigInfo
	{
		unsigned short volumeSE;//0�`255
		unsigned short volumeBGM;//0�`255
	};

	int m_volumeSE = 255;//����SE����
	int m_volumeBGM = 255;//����BGM����
public:
	~SoundManager();
	/// <summary>
	/// SoundManager�g�p�҂�GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns>���̂̎Q�Ƃ�Ԃ�</returns>
	static SoundManager& GetInstance()
	{
		static SoundManager instance;//�B��̎���
		return instance;//����̎Q�Ƃ�Ԃ�
	}

	/// <summary>
	/// �T�E���h���̃Z�[�u
	/// </summary>
	void SaveSoundConfig();

	/// <summary>
	/// �w��̃T�E���h��炷
	/// </summary>
	/// <param name="name">�T�E���h��</param>
	void Play(SoundId id, int volume = 200);
	/// <summary>
	/// BGM��炷
	/// </summary>
	/// <param name="soundH">�T�E���h�n���h��</param>
	void PlayBGM(int soundH) const;

	/// <summary>
	/// SE�̉��ʂ�ݒ肷��
	/// </summary>
	/// <param name="volume">����</param>
	void SetSEVolume(int volume);
	//SE�̉��ʂ��擾����
	int GetSEVolume()const;
	/// <summary>
	/// BGM�̉��ʂ�ݒ肷��
	/// </summary>
	/// <param name="volume">����</param>
	/// <param name="soundH">�T�E���h�n���h��</param>
	void SetBGMVolume(int volume,int soundH);
	//BGM�̉��ʂ��擾����
	int GetBGMVolume()const;

	/// <summary>
	/// �T�E���h���~�߂�
	/// </summary>
	/// <param name="id">�T�E���h��</param>
	void StopBgm(SoundId id);
};


