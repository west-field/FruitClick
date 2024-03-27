#include "Score.h"
#include <DxLib.h>
#include <cassert>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	const char* path = "Data/score.info";

	constexpr int kFontSize = 20;

	constexpr int kAdd = 5;
	constexpr int kWindowWidth = 400;
	constexpr int kWindowHeight = 300 + kAdd * 7;
	constexpr int kWindowStartX = (Game::kScreenWidth - kWindowWidth) / 2;//��
	constexpr int kWindowStartY = kFontSize;//��

	constexpr int kPosX = kWindowStartX + 10;
	constexpr int kPosY = kWindowStartY + 10;
}

Score::Score():m_point(), m_header(), m_isSave(false)
{
	//PointInit();//�����L���O������
	m_bg = my::MyLoadGraph(L"Data/panel_Example1.png");
}

Score::~Score()
{
	DeleteGraph(m_bg);
}

void Score::Draw()
{
	WindowDraw();
	
	SetFontSize(kFontSize * 2);
	int X = kPosX + ((kFontSize * 2) * 2);
	DrawString(X, kPosY, L"�����L���O", 0x191970);//���b�Z�[�W
	int y = kFontSize * 2 + 10;
	int color = 0x696969;
	for (auto& score : m_header)
	{
		if (m_isSave && score.point == m_point)
		{
			color = 0xdaa520;
		}
		else if (color != 0x696969)
		{
			color = 0x696969;
		}
		DrawFormatString(X, kPosY + y, color, L"%d�ʁF%d�_", static_cast<int>(score.rank) + 1, score.point);
		y += kFontSize * 2 + kAdd;
	}
	if (!m_isSave)
	{
		DrawFormatString(kPosX+ (kFontSize), kPosY + y, 0xdaa520, L"�����L���O�O:%d�_", m_point);
	}
	SetFontSize(0);
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
	if (m_point != score) { m_point = score; }
	m_isSave = false;
	//����̃|�C���g����ԉ��̎擾�����f�[�^�̓��_�����傫���Ƃ�
	if (score >= m_header[static_cast<int>(Rank::Max) - 1].point)
	{
		m_isSave = true;
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
	if (m_isSave)
	{
		Save();//���_�f�[�^����������
	}
}

void Score::WindowDraw()
{
	//�p�\��
	int x = kWindowStartX - 50 / 2;
	int y = kWindowStartY - 50 / 2;
	DrawRectGraph(x, y,
		0, 0, 50, 50, m_bg, true);//����@50 y3,x9
	DrawRectGraph(x + kWindowWidth, y,
		50 * 8, 0, 50, 50, m_bg, true);//�E��
	DrawRectGraph(x, y + kWindowHeight,
		0, 50 * 2, 50, 50, m_bg, true);//�����@50 y3,x9
	DrawRectGraph(x + kWindowWidth, y + kWindowHeight,
		50 * 8, 50 * 2, 50, 50, m_bg, true);//�E��

	//�摜�̍���A�E���A�O���t�B�b�N�̍��ォ��X�T�C�Y�AY�T�C�Y�A�\������摜�A����
	DrawRectExtendGraph(x + 50, y, x + kWindowWidth, y + 50,
		50 * 2, 0, 50, 50, m_bg, true);//��
	DrawRectExtendGraph(x, y + 50, x + 50, y + kWindowHeight,
		0, 50 * 1, 50, 50, m_bg, true);//��
	DrawRectExtendGraph(x + kWindowWidth, y + 50, x + kWindowWidth + 50, y + kWindowHeight,
		50 * 8, 50, 50, 50, m_bg, true);// �E
	DrawRectExtendGraph(x + 50, y + kWindowHeight, x + kWindowWidth, y + kWindowHeight + 50,
		50 * 2, 50 * 2, 50, 50, m_bg, true);	// ��
	DrawRectExtendGraph(x + 50, y + 50, x + kWindowWidth, y + kWindowHeight,
		50 * 3, 50 * 1, 50, 50, m_bg, true);	// �E�C���h�E����
}

void Score::Save()
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "wb");
	assert(fp);//�t�@�C�����J���Ȃ�������~�߂�

	fwrite(&m_header, sizeof(m_header), 1, fp);//������������

	fclose(fp);
}

void Score::PointInit()
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "wb");
	assert(fp);//�t�@�C�����J���Ȃ�������~�߂�
	//������
	Header header[static_cast<int>(Rank::Max)];
	header[static_cast<int>(Rank::First)].rank = Rank::First;
	header[static_cast<int>(Rank::First)].point = 250;
	header[static_cast<int>(Rank::Second)].rank = Rank::Second;
	header[static_cast<int>(Rank::Second)].point = 200;
	header[static_cast<int>(Rank::Third)].rank = Rank::Third;
	header[static_cast<int>(Rank::Third)].point = 150;
	header[static_cast<int>(Rank::Fourth)].rank = Rank::Fourth;
	header[static_cast<int>(Rank::Fourth)].point = 100;
	header[static_cast<int>(Rank::Fifth)].rank = Rank::Fifth;
	header[static_cast<int>(Rank::Fifth)].point = 50;
	fwrite(&header, sizeof(header), 1, fp);
	fclose(fp);
}
