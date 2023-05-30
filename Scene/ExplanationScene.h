#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <vector>

class InputState;
/// <summary>
/// 説明シーン
/// </summary>
class ExplanationScene : public Scene
{
public:
	ExplanationScene(SceneManager& manager,int selectChar);
	virtual ~ExplanationScene();

	void Update(const InputState& input,  Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_fadeColor = 0xe3e3e3;
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	//ルール説明
	void RuleExpDraw();
	//ポイント説明
	void PointExpDraw();
	//終わりの説明
	void EndExpDraw();
	//締め
	void CloseDraw();

	void (ExplanationScene::* m_updateFunc)(const InputState& ,  Mouse& );//Update用メンバ関数ポインタ
	void (ExplanationScene::* m_drawFunc)();//Draw用メンバ関数ポインタ

	Rect m_backRect;//前の表示に戻る矩形
	int m_backH;//戻る画像

	int m_selectChar;//選択したキャラクターをそのままプレイングシーンに渡すため
	int m_frameCount;//10カウントの間次のシーンに行けないようにする

	int m_bgH;//背景
	int m_scroll;//背景を動かす

	//ルール説明
	int m_hitExpH[3];
	//ポイント説明
	int m_pointExpH;
	//struct FruitInfo
	//{
	//	int handle;//フルーツ画像
	//	Position2 pos;//位置
	//	const wchar_t* name;//名前
	//	unsigned int color;//色
	//	int point;//得点
	//};
	//std::vector<FruitInfo> m_fruitsPos;//フルーツ
	//終わりの説明
	int m_endExpH[3];
	//締め
	int m_closeExpH;
};
//x224,y160
