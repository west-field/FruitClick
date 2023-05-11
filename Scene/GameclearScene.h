#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <array>

/// <summary>
/// ゲームクリアシーン
/// </summary>
class GameclearScene : public Scene
{
public:
    GameclearScene(SceneManager& manager);
    virtual ~GameclearScene();

    void Update(const InputState& input,  Mouse& mouse);
    void Draw();
private:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒

    void FadeInUpdat(const InputState& input,  Mouse& mouse);
    void FadeOutUpdat(const InputState& input,  Mouse& mouse);
    void NormalUpdat(const InputState& input,  Mouse& mouse);
    void MojiUpdate(const InputState& input,  Mouse& mouse);

    void NormalDraw();
    void MojiDraw();

    void (GameclearScene::* m_updateFunc)(const InputState&,  Mouse&);
    void (GameclearScene::* m_drawFunc)();

    struct Moji
    {
        Position2 pos = {};
        float moveY = 0.0f;
        float add = 0.0f;
    };
    static constexpr int kMojiNum = 10;
    std::array<Moji, kMojiNum> m_moji;
    const wchar_t* const kMoji[GameclearScene::kMojiNum] =
    {
        L"G",
        L"a",
        L"m",
        L"e",
        L" ",
        L"C",
        L"l",
        L"e",
        L"a",
        L"r",
    };
    int m_soundVolume = 0;
};

