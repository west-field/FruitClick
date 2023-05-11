#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"
#include "../Util/Info.h"

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
	unsigned int m_color = 0x000000;
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//�I��������̃L�����N�^�[�̓���
	void MoveChar(const InputState& input, Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	//Update�p�����o�֐��|�C���^
	void (CharacterSelectScene::* m_updateFunc)(const InputState& , Mouse& );

	static constexpr int kMenuFontSize = 50;//�����̃T�C�Y
	
	int m_selectNum = 0;//�I�����Ă���L�����N�^

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

	//�L�����N�^
	CharctorInfo m_char[static_cast<int>(CharType::Max)];

	//���̉摜�Ɉړ�����܂ł̃t���[��
	int m_frameCount;

	//���肵���L�����N�^
	int m_selectChar;

	int m_bgH;
	int m_scroll;//�w�i�𓮂���
};
