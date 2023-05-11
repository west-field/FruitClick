#pragma once
#include "Secne.h"
#include "../game.h"
#include <string>

class InputState;
class SceneManager;
/// <summary>
/// 確認シーン
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
		yes,//その処理をする
		no,//戻る

		Max
	};

	struct Element
	{
		int x, y;//座標
		int color;//色
		const wchar_t* name;//名前
		int size;
	};

	Element m_pauseMenu[static_cast<int>(Item::Max)];
	int m_selectNum = static_cast<int>(Item::no);

	std::wstring m_conf;
	bool m_isEnd;
	int m_soundH;
};

