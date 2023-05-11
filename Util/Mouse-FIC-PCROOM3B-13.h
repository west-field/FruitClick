#pragma once

#include "Geometry.h"

class InputState;

class Mouse
{
public:
	Mouse();
	virtual ~Mouse();
	void Update(const InputState& input);
	void Draw();

	//���݂̃}�E�X�ʒu�擾
	Position2 GetPos();

	//������������
	bool IsPressLeft();
	//�g���K�[����
	bool IsTriggerLeft();
	//�b��������
	bool IsRelaseLeft();

	//�������`
	Rect& GetRect();

	/// <summary>
	/// ���j���[�Z���N�g�ʒu�ɂ��邩�ǂ���
	/// </summary>
	/// <param name="startX">x���̍����W</param>
	/// <param name="endX">x���̉E���W</param>
	/// <param name="startY">y���̏���W</param>
	/// <param name="endY">y���̉����W</param>
	/// <returns>true:������ false:�����Ȃ�</returns>
	bool MouseSelect(int startX, int endX,int startY, int endY);

private:
    Rect m_rect;//�}�E�X�̈ʒu
    int m_mouseH = -1;//�}�E�X�̉摜


	static constexpr int kLogNum = 16;
	//���̓��O�@0���ŐV�̏��
	int mouseLog[kLogNum];
};

