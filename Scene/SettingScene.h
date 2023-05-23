#pragma once
#include "Secne.h"
#include "../game.h"

class InputState;
class SceneManager;
/// <summary>
/// 設定シーン
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
		pauseSound,//音量設定
		pauseBack,//戻る
		pauseGameEnd,	//ゲーム終了

		pauseMax
	};

	struct Element
	{
		int x, y;//座標
		int color;//色
		const wchar_t* name;//名前
		int size;
	};
	
	Element m_pauseMenu[static_cast<int>(Item::pauseMax)];//メニュー
	int m_selectNum = static_cast<int>(Item::pauseBack);//選択している

	int m_soundH;//BGM
};

