#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include "../Util/menuInfo.h"
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

    void Update(Mouse& mouse);
    void Draw();
private:
    void FadeInUpdat(Mouse& mouse);
    void FadeOutUpdat( Mouse& mouse);
    //得点のカウントアップ
    void NormalUpdat(Mouse& mouse);

    //メニュー処理
    void RankUpdate(Mouse& mouse);

    //得点を表示
    void NormalDraw();
    //メニューとランキングを表示
    void RankDraw();

    //得点を表示
    void PointUpdate(int leftX, int y, int dispNum, int digit = -1);

    void (GameendScene::* m_updateFunc)( Mouse&);
    void (GameendScene::* m_drawFunc)();

    unsigned int m_fadeColor = 0xe3e3e3;//フェードの色（白

    std::shared_ptr<Character> m_char;//キャラクター

    int m_soundVolume = 0;//BGM音量

    //メニュー項目
    enum MenuItem
    {
        menuGameEnd,	//ゲーム終了
        menuRestart,	//リスタート

        menuNum			//項目の数
    };

    Element m_selectMenu[menuNum];//メニュー
    int m_selectNum = 0;//選択しているメニュー項目

    int m_bgH;//背景
    int m_scroll;//背景を動かす
    int m_point;//フルーツを壊した数
    int m_pointAdd;//ポイントを足すために
    int m_pointCount;//ポイントのカウント

    int m_numFont;//数字画像
    std::shared_ptr<Score> m_score;//スコアを表示
};

