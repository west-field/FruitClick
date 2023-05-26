#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
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

    void Update(const InputState& input,  Mouse& mouse);
    void Draw();
private:
    unsigned int m_fadeColor = 0xe3e3e3;//�t�F�[�h�̐F�i��

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

    //���j���[����
    enum MenuItem
    {
        menuGameEnd,	//�Q�[���I��
        menuRestart,	//���X�^�[�g

        menuNum			//���ڂ̐�
    };
    //���j���[�v�f
    struct MenuElement
    {
        int x;
        int y;				//���W
        unsigned int color;//�F
        int fontSize;
    };
    MenuElement SelectMenu[menuNum];
    int m_selectNum = 0;//�I�����Ă��郁�j���[����

    int m_bgH;//�w�i
    int m_scroll;//�w�i�𓮂���
    int m_point;//�t���[�c���󂵂���
    int m_pointAdd;
    int m_pointCount;

    int m_numFont;//�����摜
    std::shared_ptr<Score> m_score;
};

