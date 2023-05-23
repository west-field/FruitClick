#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

class InputState;

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input, Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F�i��
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* m_updateFunc)(const InputState& , Mouse& );

	static constexpr int kMenuFontSize = 50;//�����̃T�C�Y

	//���j���[����
	enum class MenuItem
	{
		menuGameStart,	//�Q�[���X�^�[�g
		menuConfig,		//�ݒ�

		menuNum			//���ڂ̐�
	};
	
	int m_selectNum;//�I�����Ă��郁�j���[����

	int  m_moveTitle;//�������ړ�������
	int m_moveAdd;//�������ړ������鋗��

	int m_settingH ;//�ݒ�̃O���t�B�b�N
	Rect m_settingRect;//�ݒ�̋�`

	int m_bgH;//�w�i
	int m_scroll;//�w�i�𓮂���
};
