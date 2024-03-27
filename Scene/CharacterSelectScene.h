#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"
#include "../Util/Info.h"
#include "../Util/menuInfo.h"

/// <summary>
/// �L�����N�^�[�I���V�[��
/// </summary>
class CharacterSelectScene : public Scene
{
public:
	CharacterSelectScene(SceneManager& manager);
	virtual ~CharacterSelectScene();

	void Update(Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F(��
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat( Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat( Mouse& mouse);
	//�I��������̃L�����N�^�[�̓���
	void MoveChar(Mouse& mouse);
	//�ǂ̃V�[���ɕύX���邩�����߂�
	void SelectScene(Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(Mouse& mouse);

	//�ʏ�
	void NormalDraw();
	//�ǂ̃V�[���ɕύX���邩
	void SelectSceneDraw();

	/// <summary>
	/// �L�����̐�����\������
	/// </summary>
	/// <param name="type">�L�����^�C�v</param>
	/// <param name="color">�F</param>
	void DrawExplanationString(int type,int color);

	//Update�p�����o�֐��|�C���^
	void (CharacterSelectScene::* m_updateFunc)( Mouse& );
	void (CharacterSelectScene::* m_drawFunc)();

	static constexpr int kMenuFontSize = 50;//�����̃T�C�Y

	//�L�����N�^�̗v�f
	struct CharctorInfo
	{
		int handle[static_cast<int>(CharAnimType::Max)] = {};//�摜

		CharAnimType type;//���ݍĐ����Ă���A�j���[�V�����^�C�v

		int w[static_cast<int>(CharAnimType::Max)] = { 0 };//�؂�ւ���摜��
		int h = 0;//�摜��

		Rect rect = {};//�����蔻��

		int idxX = 0;//�\������摜�̈ʒuX
		int idxY = 0;//�\������摜�̈ʒuY

		bool isLeft = false;//�����ނ��Ă��邩�ǂ���
	};


	CharctorInfo m_char[static_cast<int>(CharType::Max)];	//�L�����N�^

	int m_frameCount;	//���̉摜�Ɉړ�����܂ł̃t���[��

	int m_selectCharNum = 0;//�I�����Ă���L�����N�^

	int m_selectChar;	//���肵���L�����N�^

	int m_bgH;//�w�i
	int m_scroll;//�w�i�𓮂���

	bool isExpo;//�����V�[���ɍs�����ǂ���

	/// <summary>
	/// �I��
	/// </summary>
	enum class Choice
	{
		Yes,
		No,
		Max
	};
	Element m_menu[static_cast<int>(Choice::Max)];
	int m_select;//�ǂ���̃��j���[��I�����Ă���̂�
};
