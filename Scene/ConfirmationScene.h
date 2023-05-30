#pragma once
#include "Secne.h"
#include "../game.h"
#include "../Util/menuInfo.h"
#include <string>

class InputState;
class SceneManager;

enum class SelectType
{
	End,
	BackTitle,
	SceneMode,
};

/// <summary>
/// �m�F�V�[��
/// </summary>
class ConfirmationScene : public Scene
{
public:
	ConfirmationScene(SceneManager& manager, const wchar_t* conf, SelectType type, int soundH);
	virtual ~ConfirmationScene();

	virtual void Update(const InputState& inpu, Mouse& mouse) override;
	void Draw();
private:
	void FullSceneChange();

	enum class Item
	{
		yes,//���̏���������
		no,//�߂�

		Max
	};

	Element m_pauseMenu[static_cast<int>(Item::Max)];
	int m_selectNum = static_cast<int>(Item::no);

	std::wstring m_conf;
	SelectType m_type;
	int m_soundH;

	bool m_isWindouwMode;//�E�B���h�E���[�h��ύX����
};

