#pragma once

#include <deque>

//�v���g�^�C�v�錾
class Scene;//�V�[�����N���X
class InputState;//���̓X�e�[�g
class Mouse;//�}�E�X

/// <summary>
/// �V�[���Ǘ��N���X
/// </summary>
class SceneManager
{
public:
	/// <summary>
	/// �V�[���̐؂�ւ����s��
	/// </summary>
	/// <param name="scene">�؂�ւ��������V�[���̃A�h���X</param>
	void ChangeScene(Scene* scene);

	/// <summary>
	/// �V�[������ɐς�
	/// Update�Ŏ��s�����̂͏�ɐς܂ꂽ�V�[���̂�
	/// </summary>
	void PushScene(Scene* scene);
	
	/// <summary>
	/// ��ԏ�̃V�[�����폜����
	/// �������A���̌��ʃX�^�b�N����ɂȂ邱�Ƃ̂Ȃ��悤��
	/// </summary>
	void PopScene();

	/// <summary>
	/// �e�V�[���̍X�V���s��
	/// </summary>
	/// <param name="input">���̓X�e�[�g</param>
	void Update(Mouse& mouse);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �����I�ɍ폜����
	/// </summary>
	void End();

	/// <summary>
	/// �Q�[�����I��������
	/// </summary>
	void SetIsEnd() { m_isEnd = true; }
	/// <summary>
	/// �Q�[�����I�������邩�ǂ���
	/// </summary>
	/// <returns>true:�I������ false:�I�����Ȃ�</returns>
	bool GetIsEnd() const { return m_isEnd; }

	/// <summary>
	/// �E�B���h�E���[�h���擾
	/// </summary>
	/// <returns>�@true:�E�C���h���[�h false:�t���X�N���[�����[�h</returns>
	bool GetIsWindouMode()const { return m_isWindouwMode; }
	/// <summary>
	/// �E�B���h�E���[�h��ύX����
	/// </summary>
	/// <param name="isWindou">�@true:�E�C���h���[�h false:�t���X�N���[�����[�h</param>
	void SetIsWindouMode(bool isWindou) { m_isWindouwMode = isWindou; }
private:
	//�����s���̃V�[����؂�ւ��Ă��������̂ŎQ�Ƃł͂Ȃ��|�C���^�Ƃ��Đ錾
	std::deque<Scene*> m_scenes;
	//�Q�[�����I��������
	bool m_isEnd = false;
	//�Q�[����ʃ��[�h
	bool m_isWindouwMode;
};

