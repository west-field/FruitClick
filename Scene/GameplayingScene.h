#pragma once
#include "Secne.h"
#include <memory>
#include <list>
#include "../Game/FruitsType.h"
#include "../Util/Geometry.h"

#include "../Util/Info.h"

class Character;//キャラ
class FruitsFactory;//フルーツ工場
class Spawner;//フルーツ発生器
class Stage;//ステージ

/// <summary>
/// ゲーム中シーン
/// </summary>
class GameplayingScene : public Scene
{
public:
    GameplayingScene(SceneManager& manager,int selectChar);
    virtual ~GameplayingScene();

    virtual void Update( Mouse& mouse);
    virtual void Draw();
private:
    //画面のフェードイン
     void FadeInUpdat( Mouse& mouse);
    //カウントダウン
     void CountDownUpdate(Mouse& mouse);
    //通常更新
     void NormalUpdat(Mouse& mouse);
    //画面のフェードアウト
     void FadeOutUpdat( Mouse& mouse);

     /// <summary>
     /// フルーツ生成
     /// </summary>
     void SpawnerUpdate();
     /// <summary>
     /// フルーツをランダムで作る
     /// </summary>
    void FruitsCreate(FruitsSpawnId id, const Position2 pos);
    /// <summary>
    /// 得点を表示する
    /// </summary>
    /// <param name="leftX">表示位置左</param>
    /// <param name="y">表示位置Y</param>
    /// <param name="dispNum">表示する数字</param>
    /// <param name="digit"></param>
    void PointUpdate(int leftX, int y, int dispNum, int digit = -1);

    unsigned int m_fadeColor = 0xe3e3e3;//フェードの色（白
    
    void (GameplayingScene::* m_updateFunc)( Mouse& );

    bool m_isCountDown;//最初のカウントダウンをしているかどうか
    
    int m_soundVolume = 0;//サウンドの音量

    std::shared_ptr<Character> m_char;//キャラクター

    std::shared_ptr<FruitsFactory> m_fruitsFactory;//フルーツ工場

    std::list<std::shared_ptr<Spawner>> m_spawners;//フルーツ発生器

    int m_prevFuruitsType;//一つ前に生成したフルーツ

    //ポイントを表示
    struct Point
    {
        int point;//ポイント
        Position2 pos;//表示位置
        bool isExist;//存在するかどうか
        int count = 60;//表示時間
        int fade = 255;//フェードアウト
    };
    std::list<Point> m_fruitsPoint;//フルーツのポイントを表示
    int m_point;//現在のポイント
    int m_pointAdd;//追加したいポイント
    int m_pointCount;//ポイントを追加するまでの時間

    int m_startTime;//ゲーム開始時間
    int m_fruitsFrame = 0;//次にフルーツを作るまでの時間
    
    int m_settingH = -1;//設定の画像
    Rect m_settingRect;//設定の位置

    int m_bgH;//背景ハンドル
    int m_scroll;//背景を動かす

    int m_numFont;//数字画像

    //ブロック表示用構造体
    struct Block
    {
        int handle;
        int sizeW;
        int sizeH;
        int animNum;
    };

    Block m_boxes[2];//ブロックIdleとHit2つ分を持つ
    int m_count;//経過時間
    int m_type;//今表示するブロックタイプ
    int m_idx;//ブロック表示のための変数
};

