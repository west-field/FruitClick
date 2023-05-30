#include <DxLib.h>
#include "game.h"
#include "Util/Sound.h"
#include "Util/Mouse.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"

//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	//表示画面設定
	SetUseDirectDrawDeviceIndex(1);
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);

	// windowモードを変更するため
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	auto& soundManager = SoundManager::GetInstance();

	//フォントを読み込む
	LPCWSTR font_path = L"Data/Font/PixelMplus10-Regular.ttf"; //　読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		//　フォント読込エラー処理
		MessageBox(NULL, L"フォント読込失敗", L"", MB_OK);
	}
	ChangeFont(L"PixelMplus10", DX_CHARSET_DEFAULT);
#ifdef _DEBUG
	SetMouseDispFlag(true);//マウス表示設定　false:表示しない true:表示する
#else
	SetMouseDispFlag(false);
#endif
	SetDrawScreen(DX_SCREEN_BACK);

	InputState input;
	Mouse mouse;
	SceneManager sceneManeger;
	sceneManeger.ChangeScene(new TitleScene(sceneManeger));
	sceneManeger.SetIsWindouMode(Game::kWindowMode);

	while (ProcessMessage() != -1)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		input.Update();
		mouse.Update(input);

		sceneManeger.Update(input,mouse);
		sceneManeger.Draw();

		mouse.Draw();

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		//isEndがtrueのとき終了する
		if (sceneManeger.GetIsEnd())	break;
		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667){}
	}

	//　リソースの解放
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, L"remove failure", L"", MB_OK);
	}
	//明示的に削除する
	sceneManeger.End();

	DxLib_End();

	return 0;
}