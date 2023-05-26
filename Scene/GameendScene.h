#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <array>
#include <memory>

class Character;
class Score;

/// <summary>
/// ゲーム終了シーン
/// </summary>
class GameendScene : public Scene
{
public:
    GameendScene(SceneManager& manager, std::shared_ptr<Character> character,int count);
    virtual ~GameendScene();

    void Update(const InputState& input,  Mouse& mouse);
    void Draw();
private:
    unsigned int m_fadeColor = 0xe3e3e3;//フェードの色（白

    void FadeInUpdat(const InputState& input,  Mouse& mouse);
    void FadeOutUpdat(const InputState& input,  Mouse& mouse);
    void NormalUpdat(const InputState& input,  Mouse& mouse);

    void MojiUpdate(const InputState& input, Mouse& mouse);

    void NormalDraw();
    void MojiDraw();

    void PointUpdate(int leftX, int y, int dispNum, int digit = -1);

    void (GameendScene::* m_updateFunc)(const InputState&,  Mouse&);
    void (GameendScene::* m_drawFunc)();

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
        int fontSize;
    };
    MenuElement SelectMenu[menuNum];
    int m_selectNum = 0;//選択しているメニュー項目

    int m_bgH;//背景
    int m_scroll;//背景を動かす
    int m_point;//フルーツを壊した数
    int m_pointAdd;
    int m_pointCount;

    int m_numFont;//数字画像
    std::shared_ptr<Score> m_score;
};

