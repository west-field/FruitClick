#include "Score.h"
#include <DxLib.h>
#include <cassert>
#include "../game.h"

namespace
{
	const char* path = "Data/score.info";

	static constexpr int pw_width = 400;
	static constexpr int pw_height = 300;
	static constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	static constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	static constexpr int kPosX = pw_start_x + 10;
	static constexpr int kPosY = pw_start_y + 10;

	static constexpr int kFontSize = 20;
}

Score::Score():m_point(), m_header()
{

}

Score::~Score()
{

}

void Score::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);//�E�B���h�E�Z���t�@��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	SetFontSize(kFontSize * 2);
	DrawString(pw_start_x + 10, pw_start_y + 10, L"�����L���O", 0xffff88);//���b�Z�[�W
	int y = kFontSize * 2;
	for (auto& score : m_header)
	{
		DrawFormatString(kPosX, kPosY + y, 0xffffff, L"%d�ʁF%d�_", static_cast<int>(score.rank) + 1, score.point);
		y += kFontSize * 2;
	}
	SetFontSize(0);
	
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);//�E�B���h�E�g��
}

void Score::Load()
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "rb");//�t�@�C�����J��
	assert(fp);//�t�@�C�����J���Ȃ�������~�߂�
	fread(&m_header, sizeof(m_header), 1, fp);//�擾����

	fclose(fp);//�t�@�C�������
}

void Score::Comparison(int score)
{
	bool isChange = false;//����ύX�������ǂ���

	//����̃|�C���g����ԉ��̎擾�����f�[�^�̓��_�����傫���Ƃ�
	if (score >= m_header[static_cast<int>(Rank::Max) - 1].point)
	{
		isChange = true;
		m_header[static_cast<int>(Rank::Max) - 1].point = score;
	}
	else
	{
		//��Ԃ����̓��_�����������Ƃ��͂����ŏI������B
		return;
	}

	//�����珇��5��4��3��2��1
	for (int i = static_cast<int>(Rank::Max) - 1; i >= 0; i--)
	{
		if (i - 1 < 0)	continue;
		//����̃|�C���g���擾�������_�����傫���Ƃ�
		if ( m_header[i].point > m_header[i - 1].point)
		{
			int temp;
			temp = m_header[i].point;
			m_header[i].point = m_header[i - 1].point;
			m_header[i - 1].point = temp;
		}
	}
	//����ύX�����ꍇ
	if (isChange)
	{
		Save();//���_�f�[�^����������
	}
}

void Score::Save()
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "wb");
	assert(fp);//�t�@�C�����J���Ȃ�������~�߂�

	fwrite(&m_header, sizeof(m_header), 1, fp);//������������

	fclose(fp);
}
