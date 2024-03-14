#pragma once

#include "Geometry.h"
#include <list>

class InputState;

class Mouse	final
{
public:
	Mouse();
	virtual ~Mouse();
	void Update();
	void Draw();

	void MouseReset();

	/// <summary>
	/// ���݂̃}�E�X�ʒu�擾
	/// </summary>
	/// <returns></returns>
	Position2 GetPos() const;

	//������������
	bool IsPressLeft();
	bool IsPressRight();
	//�g���K�[����
	bool IsTriggerLeft();
	bool IsTriggerRight();
	//����������
	bool IsRelaseLeft();
	bool IsRelaseRight();

	//�������`
	Rect GetRect() const;

	/// <summary>
	/// �J�[�\���̈ʒu�����j���[�Z���N�g�ʒu�ɂ��邩�ǂ���
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
	void RipplesAnimUpdate();
private:
    Rect m_rect;//�}�E�X�̈ʒu
    int m_mouseH;//�}�E�X�̉摜

	// �g���\�����邽�߂ɕK�v�ȍ\����
	struct Ripples
	{
		Position2 pos;//�ʒu
		bool isDisplay;//�\���ł��邩�ǂ���
		int idx;//�摜�̃C���f�b�N�X
	};
	
	std::list<Ripples> m_ripples;//�g��
	int m_ripplesH;//�g��摜

	static constexpr int kLogNum = 16;
	//���̓��O�@0���ŐV�̏��
	int mouseLog[kLogNum];
};

