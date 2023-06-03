#pragma once
#include "Secne.h"
#include "../game.h"
#include "../Util/menuInfo.h"

class SceneManager;
/// <summary>
/// �ݒ�V�[��
/// </summary>
class SettingScene : public Scene
{
public:
	SettingScene(SceneManager& manager, int soundH);
	virtual ~SettingScene();

	virtual void Update( Mouse& mouse) override;
	void Draw();
private:
	//�w�i�\��
	void BgDraw();

	//���j���[����
	enum class MenuItem
	{
		pauseSound,//���ʐݒ�
		pauseFullScene,//��ʕ\����؂�ւ���
		pauseBack,//�߂�
		pauseGameEnd,	//�Q�[���I��

		pauseMax
	};
	
	Element m_pauseMenu[static_cast<int>(MenuItem::pauseMax)];//���j���[
	int m_selectNum = static_cast<int>(MenuItem::pauseBack);//�I�����Ă���

	int m_soundH;//BGM

	int m_bg;//�w�i
};

