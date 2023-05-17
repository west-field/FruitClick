#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

class InputState;
/// <summary>
/// モノローグシーン
/// </summary>
class MonologueScene : public Scene
{
public:
	MonologueScene(SceneManager& manager);
	virtual ~MonologueScene();

	void Update(const InputState& input,  Mouse& mouse);
	virtual void Draw() override;

private:
	int m_selectNum = 0;//選択しているメニュー項目
	unsigned int m_fadeColor = 0xe3e3e3;
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	//Update用メンバ関数ポインタ
	void (MonologueScene::* m_updateFunc)(const InputState& ,  Mouse& );
};
//x224,y160
