#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <array>
#include <memory>

class Character;

/// <summary>
/// �Q�[���N���A�V�[��
/// </summary>
class GameclearScene : public Scene
{
public:
    GameclearScene(SceneManager& manager, std::shared_ptr<Character> character);
    virtual ~GameclearScene();

    void Update(const InputState& input,  Mouse& mouse);
    void Draw();
private:
    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input,  Mouse& mouse);
    void FadeOutUpdat(const InputState& input,  Mouse& mouse);
    void NormalUpdat(const InputState& input,  Mouse& mouse);

    void (GameclearScene::* m_updateFunc)(const InputState&,  Mouse&);

    std::shared_ptr<Character> m_char;

    int m_soundVolume = 0;

    int m_bgH;
    int m_scroll;//�w�i�𓮂���
};

