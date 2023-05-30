#pragma once
#include "Secne.h"
#include "../game.h"

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
	enum class Item
	{
		pauseSound,//���ʐݒ�
		pauseFullscreen,//��ʕ\����؂�ւ���
		pauseBack,//�߂�
		pauseTitle,//�^�C�g���ɖ߂�
		pauseGameEnd,//�Q�[�����I���

		pauseMax
	};

	struct Element
	{
		int x, y;//���W
		int color;//�F
		const wchar_t* name;//���O
		int size;
	};
	
	Element m_pauseMenu[static_cast<int>(Item::pauseMax)];
	int m_selectNum = static_cast<int>(Item::pauseBack);

	int m_soundH;
};

