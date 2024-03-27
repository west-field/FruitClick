#pragma once
#include "Secne.h"
#include "../game.h"
#include "../Util/menuInfo.h"

/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene : public Scene
{
public:
	PauseScene(SceneManager& manager, int soundH);
	virtual ~PauseScene();

	virtual void Update( Mouse& mouse) override;
	void Draw();
private:
	/// <summary>
	/// 背景を表示
	/// </summary>
	void WindowDraw();

	/// <summary>
	/// 項目
	/// </summary>
	enum class Item
	{
		pauseSound,//音量設定
		pauseFullscreen,//画面表示を切り替える
		pauseBack,//戻る
		pauseTitle,//タイトルに戻る
		pauseGameEnd,//ゲームを終わる

		pauseMax
	};

	Element m_pauseMenu[static_cast<int>(Item::pauseMax)];
	int m_selectNum;

	int m_soundH;

	int m_bg;
};

