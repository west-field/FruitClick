#pragma once
#include "Secne.h"
#include "../game.h"
#include "../Util/menuInfo.h"

class SceneManager;
/// <summary>
/// 設定シーン
/// </summary>
class SettingScene : public Scene
{
public:
	SettingScene(SceneManager& manager, int soundH);
	virtual ~SettingScene();

	virtual void Update( Mouse& mouse) override;
	void Draw();
private:
	//背景表示
	void BgDraw();

	//メニュー項目
	enum class MenuItem
	{
		pauseSound,//音量設定
		pauseFullScene,//画面表示を切り替える
		pauseBack,//戻る
		pauseGameEnd,	//ゲーム終了

		pauseMax
	};
	
	Element m_pauseMenu[static_cast<int>(MenuItem::pauseMax)];//メニュー
	int m_selectNum = static_cast<int>(MenuItem::pauseBack);//選択している

	int m_soundH;//BGM

	int m_bg;//背景
};

