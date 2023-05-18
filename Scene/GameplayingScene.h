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

    virtual void Update(const InputState& input, Mouse& mouse);
    virtual void Draw();
private:
    //画面のフェードイン
     void FadeInUpdat(const InputState& input,  Mouse& mouse);
    //通常更新
     void NormalUpdat(const InputState& input,  Mouse& mouse);
    //画面のフェードアウト
     void FadeOutUpdat(const InputState& input,  Mouse& mouse);

     void SpawnerUpdate();
     /// <summary>
     /// フルーツをランダムで作る
     /// </summary>
    void FruitsCreate(FruitsSpawnId id, const Position2 pos);

    void PointUpdate(int leftX, int y, int dispNum, int digit = -1);

    unsigned int m_fadeColor = 0xe3e3e3;//フェードの色（白
    
    void (GameplayingScene::* m_updateFunc)(const InputState& ,  Mouse& );

    int m_bossBgm = -1;//ボス音楽
    
    int m_soundVolume = 0;//サウンドの音量

    std::shared_ptr<Character> m_char;//キャラクタ

    std::shared_ptr<FruitsFactory> m_fruitsFactory;//フルーツ工場

    std::list<std::shared_ptr<Spawner>> m_spawners;//フルーツ発生器

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

    Block m_blocks[2];//ブロックIdleとHit2つ分を持つ
    int m_count;//経過時間
    int m_type;//今表示するブロックタイプ
    int m_idx;//ブロック表示のための変数
};

