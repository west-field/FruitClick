#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <vector>

class InputState;
/// <summary>
/// �����V�[��
/// </summary>
class ExplanationScene : public Scene
{
public:
	ExplanationScene(SceneManager& manager,int selectChar);
	virtual ~ExplanationScene();

	void Update(const InputState& input,  Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_fadeColor = 0xe3e3e3;
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	//���[������
	void RuleExpDraw();
	//�|�C���g����
	void PointExpDraw();
	//�I���̐���
	void EndExpDraw();
	//����
	void CloseDraw();

	void (ExplanationScene::* m_updateFunc)(const InputState& ,  Mouse& );//Update�p�����o�֐��|�C���^
	void (ExplanationScene::* m_drawFunc)();//Draw�p�����o�֐��|�C���^

	Rect m_backRect;//�O�̕\���ɖ߂��`
	int m_backH;//�߂�摜

	int m_selectChar;//�I�������L�����N�^�[�����̂܂܃v���C���O�V�[���ɓn������
	int m_frameCount;//10�J�E���g�̊Ԏ��̃V�[���ɍs���Ȃ��悤�ɂ���

	int m_bgH;//�w�i
	int m_scroll;//�w�i�𓮂���

	//���[������
	int m_hitExpH[3];
	//�|�C���g����
	int m_pointExpH;
	//struct FruitInfo
	//{
	//	int handle;//�t���[�c�摜
	//	Position2 pos;//�ʒu
	//	const wchar_t* name;//���O
	//	unsigned int color;//�F
	//	int point;//���_
	//};
	//std::vector<FruitInfo> m_fruitsPos;//�t���[�c
	//�I���̐���
	int m_endExpH[3];
	//����
	int m_closeExpH;
};
//x224,y160
