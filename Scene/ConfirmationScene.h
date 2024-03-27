#pragma once
#include "Secne.h"
#include "../game.h"
#include "../Util/menuInfo.h"
#include <string>

/// <summary>
/// 次に何をするのか
/// </summary>
enum class SelectType
{
	End,
	BackTitle,
	SceneMode,
};

/// <summary>
/// 確認シーン
/// </summary>
class ConfirmationScene : public Scene
{
public:
	ConfirmationScene(SceneManager& manager, const wchar_t* conf, SelectType type, int soundH);
	virtual ~ConfirmationScene();

	virtual void Update(Mouse& mouse) override;
	void Draw();
private:
	/// <summary>
	/// 背景のウィンドウを表示する
	/// </summary>
	void WindowDraw();
	/// <summary>
	/// ウィンドウモードを変更する
	/// </summary>
	void WindowModeChange();

	/// <summary>
	/// 選択
	/// </summary>
	enum class Choice
	{
		yes,//その処理をする
		no,//戻る

		Max
	};

	Element m_pauseMenu[static_cast<int>(Choice::Max)];
	int m_selectNum = static_cast<int>(Choice::no);

	const wchar_t* m_conf;//表示する文字
	int m_stringNum;//表示する文字の数
	SelectType m_type;//次何をするかを指定
	int m_soundH;//音を次のシーンに渡すため

	bool m_isWindowMode;//ウィンドウモードを変更する

	int m_bg;//ウィンドウ背景
};

