#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <vector>

/// <summary>
/// 説明シーン
/// </summary>
class ExplanationScene : public Scene
{
public:
	ExplanationScene(SceneManager& manager,int selectChar);
	virtual ~ExplanationScene();

	void Update( Mouse& mouse);
	virtual void Draw() override;

private:
	//フェードインの時のUpdate関数
	void FadeInUpdat(Mouse& mouse);
	//通常状態の時のUpdate関数
	void NormalUpdat(Mouse& mouse);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(Mouse& mouse);

	//ルール説明
	void RuleExpDraw();
	//ポイント説明
	void PointExpDraw();
	//終わりの説明
	void EndExpDraw();
	//締め
	void CloseDraw();

	void (ExplanationScene::* m_updateFunc)(Mouse& );//Update用メンバ関数ポインタ
	void (ExplanationScene::* m_drawFunc)();//Draw用メンバ関数ポインタ

	unsigned int m_fadeColor = 0xe3e3e3;//フェードイン、フェードアウト時の色

	Rect m_backRect;//前の表示に戻る矩形
	int m_backH;//戻る画像

	int m_selectChar;//選択したキャラクターをそのままプレイングシーンに渡すため
	int m_frameCount;//10カウントの間次のシーンに行けないようにする

	int m_bgH;//背景
	int m_scroll;//背景を動かす

	int m_hitExpH[3];//ルール説明
	
	int m_pointExpH;//ポイント説明
	
	int m_endExpH[3];//終わりの説明
	
	int m_closeExpH;//締め
};

