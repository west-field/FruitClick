#pragma once

#include "Geometry.h"
#include <list>

class InputState;

class Mouse
{
public:
	Mouse();
	virtual ~Mouse();
	void Update(const InputState& input);
	void Draw();

	void MouseReset();

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
	/// <param name="startY">y���̏���W</param>
	/// <param name="endY">y���̉����W</param>
	/// <param name="startY">y���̏���W</param>
	/// <param name="endY">y���̉����W</param>
	/// <returns>true:������ false:�����Ȃ�</returns>
	bool MouseSelect(int startX, int endX,int startY, int endY);

	bool MouseSelect(float startX, float endX,float startY, float endY);

	/// <summary>
	/// �N���b�N�����Ƃ��ɏo���g����쐬
	/// </summary>
	void ClickAnimCreate();

	/// <summary>
	/// �N���b�N�����Ƃ��ɏo���g��̍X�V
	/// </summary>
	void ClickAnimUpdate();
private:
    Rect m_rect;//�}�E�X�̈ʒu
    int m_mouseH;//�}�E�X�̉摜

	struct Click
	{
		Position2 pos;
		bool isClick;
		int idx;
	};
	std::list<Click> m_click;
	int m_clickH;

	static constexpr int kLogNum = 16;
	//���̓��O�@0���ŐV�̏��
	int mouseLog[kLogNum];
};

