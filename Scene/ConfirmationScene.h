#pragma once
#include "Secne.h"
#include "../game.h"
#include <string>

class InputState;
class SceneManager;
/// <summary>
/// �m�F�V�[��
/// </summary>
class ConfirmationScene : public Scene
{
public:
	ConfirmationScene(SceneManager& manager, const wchar_t* conf,bool isEnd, int soundH);
	virtual ~ConfirmationScene();

	virtual void Update(const InputState& inpu, Mouse& mouse) override;
	void Draw();
private:
	enum class Item
	{
		yes,//���̏���������
		no,//�߂�

		Max
	};

	struct Element
	{
		int x, y;//���W
		int color;//�F
		const wchar_t* name;//���O
		int size;
	};

	Element m_pauseMenu[static_cast<int>(Item::Max)];
	int m_selectNum = static_cast<int>(Item::no);

	std::wstring m_conf;
	bool m_isEnd;
	int m_soundH;
};

