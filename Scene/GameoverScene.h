#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include "../game.h"

#include <array>


/// <summary>
/// �Q�[���I�[�o�[�V�[��
/// </summary>
class GameoverScene : public Scene
{
public:
    GameoverScene(SceneManager& manager);
    virtual ~GameoverScene();

    void Update(const InputState& input,  Mouse& mouse);
    void Draw();
private:
    unsigned int m_fadeColor = 0xff0000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input,  Mouse& mouse);
    void FadeOutUpdat(const InputState& input,  Mouse& mouse);
    void NormalUpdat(const InputState& input,  Mouse& mouse);
    void MojiUpdate(const InputState& input,  Mouse& mouse);

    void NormalDraw();
    void MojiDraw();

    void (GameoverScene::*m_updateFunc )(const InputState&,  Mouse&) ;
    void (GameoverScene::*m_drawFunc )() ;

    //����
    struct Moji
    {
        Position2 pos = {};
        float moveY = 0.0f;
        float add = 0.0f;
    };
    
    static constexpr int kMojiNum = 9;
    std::array<Moji, kMojiNum> m_moji;
    const wchar_t* const kMoji[GameoverScene::kMojiNum] =
    {
        L"G",
        L"a",
        L"m",
        L"e",
        L" ",
        L"O",
        L"v",
        L"e",
        L"r",
    };
    int  m_soundVolume = 0;

    static constexpr int kMenuFontSize = 50;//�����̃T�C�Y
    static constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize * 2;    //���j���[������x�ʒu
    static constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize * 2;    //���j���[������y�ʒu
    static constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//���j���[�����̈ړ�����x�ʒu
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
    };
    MenuElement SelectMenu[menuNum] = {
        { kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameEnd,0xffa0aa  },
        { kOriginalPosX, kOriginalPosY + kMenuFontSize * menuRestart + 5,0xaaa0ff}
    };
    int m_selectNum = 0;//�I�����Ă��郁�j���[����
};

