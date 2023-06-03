#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(Mouse& mouse);
	virtual void Draw() override;

private:
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat( Mouse& mouse);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* m_updateFunc)(Mouse& );

	unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F�i��

	//���j���[����
	enum class MenuItem
	{
		menuGameStart,	//�Q�[���X�^�[�g
		menuConfig,		//�ݒ�

		menuNum			//���ڂ̐�
	};
	
	int m_selectNum;//�I�����Ă��郁�j���[����

	int  m_instrString;//�������ړ�������
	int m_moveAdd;//�������ړ������鋗��

	int m_settingH ;//�ݒ�̃O���t�B�b�N
	Rect m_settingRect;//�ݒ�̋�`

	int m_bgH;//�w�i
	int m_scroll;//�w�i�𓮂���

	Position2 m_charPos;
	int m_char[4];//�L�����N�^�[��\��
	int m_charType;//�\������L�����N�^�[
	int m_frameCount;//�摜�ύX
	int m_idx;
};
