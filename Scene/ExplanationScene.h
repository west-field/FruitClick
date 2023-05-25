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

	void NormalDraw();
	void FirstExpDraw();
	void PointExpDraw();
	void ThirdExpDraw();
	void FourthExpDraw();

	//Update用メンバ関数ポインタ
	void (ExplanationScene::* m_updateFunc)(const InputState& ,  Mouse& );
	void (ExplanationScene::* m_drawFunc)();

	Rect m_backRect;
	int m_backH;

	int m_selectChar;
	int m_frameCount;

	int m_bgH;//背景
	int m_scroll;//背景を動かす

	int m_fruitH;//フルーツ画像
	struct FruitInfo
	{
		Position2 pos;//位置
		const wchar_t* name;//名前
		unsigned int color;//色
		int point;//得点
	};
	std::vector<FruitInfo> m_fruitsPos;//フルーツ
};
//x224,y160
