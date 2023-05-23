#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"

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

	//Update�p�����o�֐��|�C���^
	void (ExplanationScene::* m_updateFunc)(const InputState& ,  Mouse& );

	int m_selectChar;
};
//x224,y160
