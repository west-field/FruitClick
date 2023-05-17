#pragma once
#include "Secne.h"
#include <memory>
#include <list>
#include "../Game/FruitsType.h"
#include "../Util/Geometry.h"

#include "../Util/Info.h"

class Character;//�L����
class FruitsFactory;//�t���[�c�H��
class Spawner;//�t���[�c������
class Stage;//�X�e�[�W

/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene
{
public:
    GameplayingScene(SceneManager& manager,int selectChar);
    virtual ~GameplayingScene();

    virtual void Update(const InputState& input, Mouse& mouse);
    virtual void Draw();
private:
    //��ʂ̃t�F�[�h�C��
     void FadeInUpdat(const InputState& input,  Mouse& mouse);
    //�ʏ�X�V
     void NormalUpdat(const InputState& input,  Mouse& mouse);
    //��ʂ̃t�F�[�h�A�E�g
     void FadeOutUpdat(const InputState& input,  Mouse& mouse);

     void SpawnerUpdate();
     /// <summary>
     /// �t���[�c�������_���ō��
     /// </summary>
    void FruitsCreate(FruitsSpawnId id, const Position2 pos);

    void PointUpdate(int leftX, int y, int dispNum, int digit = -1);

    unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F�i��
    
    void (GameplayingScene::* m_updateFunc)(const InputState& ,  Mouse& );

    int m_bossBgm = -1;//�{�X���y
    
    int m_soundVolume = 0;//�T�E���h�̉���

    std::shared_ptr<Character> m_char;//�L�����N�^

    std::shared_ptr<FruitsFactory> m_fruitsFactory;//�t���[�c�H��

    std::list<std::shared_ptr<Spawner>> m_spawners;//�t���[�c������

    int m_fruitsFrame = 0;//���Ƀt���[�c�����܂ł̎���
    
    int m_settingH = -1;//�ݒ�̉摜
    Rect m_settingRect;//�ݒ�̈ʒu

    std::shared_ptr<Stage> m_stage;
    int m_bgH;
    int m_scroll;//�w�i�𓮂���
    int m_numFont;//�����摜
};

