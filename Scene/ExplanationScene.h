#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"

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

	//Update用メンバ関数ポインタ
	void (ExplanationScene::* m_updateFunc)(const InputState& ,  Mouse& );

	int m_selectChar;
};
//x224,y160
