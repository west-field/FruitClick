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

    unsigned int m_fadeColor = 0x000000;//フェードの色（黒
    
    void (GameplayingScene::* m_updateFunc)(const InputState& ,  Mouse& );

    int m_bossBgm = -1;//ボス音楽
    
    int m_soundVolume = 0;//サウンドの音量
    
    int m_crea = 0;//クリア

    std::shared_ptr<Character> m_char;//キャラクタ

    std::shared_ptr<FruitsFactory> m_fruitsFactory;//フルーツ工場

    std::list<std::shared_ptr<Spawner>> m_spawners;//フルーツ発生器
    int m_fruitsFrame = 0;//次にフルーツを作るまでの時間
    
    int m_settingH = -1;//設定の画像
    Rect m_settingRect;//設定の位置


    int m_bgH;
};

