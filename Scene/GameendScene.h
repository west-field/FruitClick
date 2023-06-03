#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include "../Util/menuInfo.h"
#include <array>
#include <memory>

class Character;
class Score;

/// <summary>
/// �Q�[���I���V�[��
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
    //���_�̃J�E���g�A�b�v
    void NormalUpdat(Mouse& mouse);

    //���j���[����
    void RankUpdate(Mouse& mouse);

    //���_��\��
    void NormalDraw();
    //���j���[�ƃ����L���O��\��
    void RankDraw();

    //���_��\��
    void PointUpdate(int leftX, int y, int dispNum, int digit = -1);

    void (GameendScene::* m_updateFunc)( Mouse&);
    void (GameendScene::* m_drawFunc)();

    unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F�i��

    std::shared_ptr<Character> m_char;//�L�����N�^�[

    int m_soundVolume = 0;//BGM����

    //���j���[����
    enum MenuItem
    {
        menuGameEnd,	//�Q�[���I��
        menuRestart,	//���X�^�[�g

        menuNum			//���ڂ̐�
    };

    Element m_selectMenu[menuNum];//���j���[
    int m_selectNum = 0;//�I�����Ă��郁�j���[����

    int m_bgH;//�w�i
    int m_scroll;//�w�i�𓮂���
    int m_point;//�t���[�c���󂵂���
    int m_pointAdd;//�|�C���g�𑫂����߂�
    int m_pointCount;//�|�C���g�̃J�E���g

    int m_numFont;//�����摜
    std::shared_ptr<Score> m_score;//�X�R�A��\��
};

