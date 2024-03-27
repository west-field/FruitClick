#pragma once
#include "Secne.h"
#include "../game.h"
#include "../Util/menuInfo.h"

class InputState;
class SceneManager;
/// <summary>
/// �|�[�Y�V�[��
/// </summary>
class PauseScene : public Scene
{
public:
	PauseScene(SceneManager& manager, int soundH);
	virtual ~PauseScene();

	virtual void Update(const InputState& input, Mouse& mouse) override;
	void Draw();
private:
	void BgDraw();

	enum class Item
	{
		pauseSound,//���ʐݒ�
		pauseFullscreen,//��ʕ\����؂�ւ���
		pauseBack,//�߂�
		pauseTitle,//�^�C�g���ɖ߂�
		pauseGameEnd,//�Q�[�����I���

		pauseMax
	};

	Element m_pauseMenu[static_cast<int>(Item::pauseMax)];
	int m_selectNum;

	int m_soundH;

	int m_bg;
};
