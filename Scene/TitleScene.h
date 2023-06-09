#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(Mouse& mouse);
	virtual void Draw() override;

private:
	//フェードインの時のUpdate関数
	void FadeInUpdat(Mouse& mouse);
	//通常状態の時のUpdate関数
	void NormalUpdat(Mouse& mouse);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat( Mouse& mouse);

	//Update用メンバ関数ポインタ
	void (TitleScene::* m_updateFunc)(Mouse& );

	unsigned int m_fadeColor = 0xe3e3e3;//フェードの色（白

	//メニュー項目
	enum class MenuItem
	{
		menuGameStart,	//ゲームスタート
		menuConfig,		//設定

		menuNum			//項目の数
	};
	
	int m_selectNum;//選択しているメニュー項目

	int  m_instrString;//文字を移動させる
	int m_moveAdd;//文字を移動させる距離

	int m_settingH ;//設定のグラフィック
	Rect m_settingRect;//設定の矩形

	int m_bgH;//背景
	int m_scroll;//背景を動かす

	Position2 m_charPos;
	int m_char[4];//キャラクターを表示
	int m_charType;//表示するキャラクター
	int m_frameCount;//画像変更
	int m_idx;
};
