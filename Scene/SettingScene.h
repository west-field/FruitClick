#pragma once
#include "Secne.h"
#include "../game.h"

class InputState;
class SceneManager;
/// <summary>
/// �ݒ�V�[��
/// </summary>
class SettingScene : public Scene
{
public:
	SettingScene(SceneManager& manager, int soundH);
	virtual ~SettingScene();

	virtual void Update(const InputState& inpu, Mouse& mouse) override;
	void Draw();
private:
	enum class Item
	{
		pauseSound,//���ʐݒ�
		pauseBack,//�߂�
		pauseGameEnd,	//�Q�[���I��

		pauseMax
	};

	struct Element
	{
		int x, y;//���W
		int color;//�F
		const wchar_t* name;//���O
		int size;
	};
	
	Element m_pauseMenu[static_cast<int>(Item::pauseMax)];//���j���[
	int m_selectNum = static_cast<int>(Item::pauseBack);//�I�����Ă���

	int m_soundH;//BGM
};

