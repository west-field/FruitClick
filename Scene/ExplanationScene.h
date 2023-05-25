#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <vector>

class InputState;
/// <summary>
/// �����V�[��
/// </summary>
class ExplanationScene : public Scene
{
public:
	ExplanationScene(SceneManager& manager,int selectChar);
	virtual ~ExplanationScene();

	void Update(const InputState& input,  Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_fadeColor = 0xe3e3e3;
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	void NormalDraw();
	void FirstExpDraw();
	void PointExpDraw();
	void ThirdExpDraw();
	void FourthExpDraw();

	//Update�p�����o�֐��|�C���^
	void (ExplanationScene::* m_updateFunc)(const InputState& ,  Mouse& );
	void (ExplanationScene::* m_drawFunc)();

	Rect m_backRect;
	int m_backH;

	int m_selectChar;
	int m_frameCount;

	int m_bgH;//�w�i
	int m_scroll;//�w�i�𓮂���

	int m_fruitH;//�t���[�c�摜
	struct FruitInfo
	{
		Position2 pos;//�ʒu
		const wchar_t* name;//���O
		unsigned int color;//�F
		int point;//���_
	};
	std::vector<FruitInfo> m_fruitsPos;//�t���[�c
};
//x224,y160
