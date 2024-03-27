#pragma once
#include "Secne.h"
#include "../game.h"
#include "../Util/menuInfo.h"
#include <string>

/// <summary>
/// ���ɉ�������̂�
/// </summary>
enum class SelectType
{
	End,
	BackTitle,
	SceneMode,
};

/// <summary>
/// �m�F�V�[��
/// </summary>
class ConfirmationScene : public Scene
{
public:
	ConfirmationScene(SceneManager& manager, const wchar_t* conf, SelectType type, int soundH);
	virtual ~ConfirmationScene();

	virtual void Update(Mouse& mouse) override;
	void Draw();
private:
	/// <summary>
	/// �w�i�̃E�B���h�E��\������
	/// </summary>
	void WindowDraw();
	/// <summary>
	/// �E�B���h�E���[�h��ύX����
	/// </summary>
	void WindowModeChange();

	/// <summary>
	/// �I��
	/// </summary>
	enum class Choice
	{
		yes,//���̏���������
		no,//�߂�

		Max
	};

	Element m_pauseMenu[static_cast<int>(Choice::Max)];
	int m_selectNum = static_cast<int>(Choice::no);

	const wchar_t* m_conf;//�\�����镶��
	int m_stringNum;//�\�����镶���̐�
	SelectType m_type;//���������邩���w��
	int m_soundH;//�������̃V�[���ɓn������

	bool m_isWindowMode;//�E�B���h�E���[�h��ύX����

	int m_bg;//�E�B���h�E�w�i
};

