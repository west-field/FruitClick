#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <vector>

/// <summary>
/// �����V�[��
/// </summary>
class ExplanationScene : public Scene
{
public:
	ExplanationScene(SceneManager& manager,int selectChar);
	virtual ~ExplanationScene();

	void Update( Mouse& mouse);
	virtual void Draw() override;

private:
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(Mouse& mouse);

	//���[������
	void RuleExpDraw();
	//�|�C���g����
	void PointExpDraw();
	//�I���̐���
	void EndExpDraw();
	//����
	void CloseDraw();

	void (ExplanationScene::* m_updateFunc)(Mouse& );//Update�p�����o�֐��|�C���^
	void (ExplanationScene::* m_drawFunc)();//Draw�p�����o�֐��|�C���^

	unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�C���A�t�F�[�h�A�E�g���̐F

	Rect m_backRect;//�O�̕\���ɖ߂��`
	int m_backH;//�߂�摜

	int m_selectChar;//�I�������L�����N�^�[�����̂܂܃v���C���O�V�[���ɓn������
	int m_frameCount;//10�J�E���g�̊Ԏ��̃V�[���ɍs���Ȃ��悤�ɂ���

	int m_bgH;//�w�i
	int m_scroll;//�w�i�𓮂���

	int m_hitExpH[3];//���[������
	
	int m_pointExpH;//�|�C���g����
	
	int m_endExpH[3];//�I���̐���
	
	int m_closeExpH;//����
};

