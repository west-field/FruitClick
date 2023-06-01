#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"
#include "../Util/Info.h"
#include "../Util/menuInfo.h"

class InputState;

/// <summary>
/// �L�����N�^�[�I���V�[��
/// </summary>
class CharacterSelectScene : public Scene
{
public:
	CharacterSelectScene(SceneManager& manager);
	virtual ~CharacterSelectScene();

	void Update(const InputState& input, Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F(��
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//�I��������̃L�����N�^�[�̓���
	void MoveChar(const InputState& input, Mouse& mouse);
	//�ǂ̃V�[���ɕύX���邩�����߂�
	void SelectScene(const InputState& input, Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

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
	void (CharacterSelectScene::* m_updateFunc)(const InputState& , Mouse& );
	void (CharacterSelectScene::* m_drawFunc)();

	static constexpr int kMenuFontSize = 50;//�����̃T�C�Y

	//�L�����N�^�̗v�f
	struct CharctorInfo
	{
		int handle[static_cast<int>(CharAnimType::Max)] = {};

		CharAnimType type;

		int w[static_cast<int>(CharAnimType::Max)] = { 0 };
		int h = 0;

		Rect rect = {};

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

	enum class Item
	{
		Yes,
		No,
		Max
	};
	Element m_menu[static_cast<int>(Item::Max)];
	int m_select;
};
