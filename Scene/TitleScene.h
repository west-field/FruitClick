#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

class InputState;

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input, Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_color = 0x000000;
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	//Update用メンバ関数ポインタ
	void (TitleScene::* m_updateFunc)(const InputState& , Mouse& );

	static constexpr int kMenuFontSize = 50;//文字のサイズ

	//メニュー項目
	enum class MenuItem
	{
		menuGameStart,	//ゲームスタート
		menuConfig,		//設定

		menuNum			//項目の数
	};
	
	int m_selectNum;//選択しているメニュー項目

	int  m_moveTitle;
	int m_moveAdd;

	int m_settingH ;
	Rect m_settingRect;


	int m_bgH;
	int m_scroll;//背景を動かす
};
