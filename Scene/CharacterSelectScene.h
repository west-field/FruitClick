#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"
#include "../Util/Info.h"

class InputState;

/// <summary>
/// キャラクター選択シーン
/// </summary>
class CharacterSelectScene : public Scene
{
public:
	CharacterSelectScene(SceneManager& manager);
	virtual ~CharacterSelectScene();

	void Update(const InputState& input, Mouse& mouse);
	virtual void Draw() override;

private:
	unsigned int m_fadeColor = 0xe3e3e3;//フェードの色(白
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input,  Mouse& mouse);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input,  Mouse& mouse);
	//選択した後のキャラクターの動き
	void MoveChar(const InputState& input, Mouse& mouse);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input,  Mouse& mouse);

	/// <summary>
	/// キャラの説明を表示する
	/// </summary>
	/// <param name="type">キャラタイプ</param>
	/// <param name="color">色</param>
	void DrawExplanationString(int type,int color);

	//Update用メンバ関数ポインタ
	void (CharacterSelectScene::* m_updateFunc)(const InputState& , Mouse& );

	static constexpr int kMenuFontSize = 50;//文字のサイズ
	
	int m_selectNum = 0;//選択しているキャラクタ

	struct CharctorInfo
	{
		int handle[static_cast<int>(CharAnimType::Max)] = {};

		CharAnimType type;

		int w[static_cast<int>(CharAnimType::Max)] = { 0 };
		int h = 0;

		Rect rect = {};

		int idxX = 0;//表示する画像の位置X
		int idxY = 0;//表示する画像の位置Y

		bool isLeft = false;//左をむいているかどうか
	};

	//キャラクタ
	CharctorInfo m_char[static_cast<int>(CharType::Max)];

	//次の画像に移動するまでのフレーム
	int m_frameCount;

	//決定したキャラクタ
	int m_selectChar;

	int m_bgH;
	int m_scroll;//背景を動かす
};
