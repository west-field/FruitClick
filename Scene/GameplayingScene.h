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

    virtual void Update( Mouse& mouse);
    virtual void Draw();
private:
    //��ʂ̃t�F�[�h�C��
     void FadeInUpdat( Mouse& mouse);
    //�J�E���g�_�E��
     void CountDownUpdate(Mouse& mouse);
    //�ʏ�X�V
     void NormalUpdat(Mouse& mouse);
    //��ʂ̃t�F�[�h�A�E�g
     void FadeOutUpdat( Mouse& mouse);

     /// <summary>
     /// �t���[�c����
     /// </summary>
     void SpawnerUpdate();
     /// <summary>
     /// �t���[�c�������_���ō��
     /// </summary>
    void FruitsCreate(FruitsSpawnId id, const Position2 pos);
    /// <summary>
    /// ���_��\������
    /// </summary>
    /// <param name="leftX">�\���ʒu��</param>
    /// <param name="y">�\���ʒuY</param>
    /// <param name="dispNum">�\�����鐔��</param>
    /// <param name="digit"></param>
    void PointUpdate(int leftX, int y, int dispNum, int digit = -1);

    unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F�i��
    
    void (GameplayingScene::* m_updateFunc)( Mouse& );

    bool m_isCountDown;//�ŏ��̃J�E���g�_�E�������Ă��邩�ǂ���
    
    int m_soundVolume = 0;//�T�E���h�̉���

    std::shared_ptr<Character> m_char;//�L�����N�^�[

    std::shared_ptr<FruitsFactory> m_fruitsFactory;//�t���[�c�H��

    std::list<std::shared_ptr<Spawner>> m_spawners;//�t���[�c������

    int m_prevFuruitsType;//��O�ɐ��������t���[�c

    //�|�C���g��\��
    struct Point
    {
        int point;//�|�C���g
        Position2 pos;//�\���ʒu
        bool isExist;//���݂��邩�ǂ���
        int count = 60;//�\������
        int fade = 255;//�t�F�[�h�A�E�g
    };
    std::list<Point> m_fruitsPoint;//�t���[�c�̃|�C���g��\��
    int m_point;//���݂̃|�C���g
    int m_pointAdd;//�ǉ��������|�C���g
    int m_pointCount;//�|�C���g��ǉ�����܂ł̎���

    int m_startTime;//�Q�[���J�n����
    int m_fruitsFrame = 0;//���Ƀt���[�c�����܂ł̎���
    
    int m_settingH = -1;//�ݒ�̉摜
    Rect m_settingRect;//�ݒ�̈ʒu

    int m_bgH;//�w�i�n���h��
    int m_scroll;//�w�i�𓮂���

    int m_numFont;//�����摜

    //�u���b�N�\���p�\����
    struct Block
    {
        int handle;
        int sizeW;
        int sizeH;
        int animNum;
    };

    Block m_boxes[2];//�u���b�NIdle��Hit2��������
    int m_count;//�o�ߎ���
    int m_type;//���\������u���b�N�^�C�v
    int m_idx;//�u���b�N�\���̂��߂̕ϐ�
};

