#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <array>
#include <memory>

class Character;

/// <summary>
/// ゲームクリアシーン
/// </summary>
class GameclearScene : public Scene
{
public:
    GameclearScene(SceneManager& manager, std::shared_ptr<Character> character);
    virtual ~GameclearScene();

    void Update(const InputState& input,  Mouse& mouse);
    void Draw();
private:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒

    void FadeInUpdat(const InputState& input,  Mouse& mouse);
    void FadeOutUpdat(const InputState& input,  Mouse& mouse);
    void NormalUpdat(const InputState& input,  Mouse& mouse);

    void MojiUpdate(const InputState& input, Mouse& mouse);

    void NormalDraw();
    void MojiDraw();

    void (GameclearScene::* m_updateFunc)(const InputState&,  Mouse&);
    void (GameclearScene::* m_drawFunc)();

    std::shared_ptr<Character> m_char;

    int m_soundVolume = 0;

    //メニュー項目
    enum MenuItem
    {
        menuGameEnd,	//ゲーム終了
        menuRestart,	//リスタート

        menuNum			//項目の数
    };
    //メニュー要素
    struct MenuElement
    {
        int x;
        int y;				//座標
        unsigned int color;//色
        int size;
    };
    MenuElement SelectMenu[menuNum];
    int m_selectNum = 0;//選択しているメニュー項目

    int m_bgH;
    int m_scroll;//背景を動かす
};

