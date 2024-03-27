#pragma once

#include <deque>

//プロトタイプ宣言
class Scene;//シーン基底クラス
class InputState;//入力ステート
class Mouse;//マウス

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager
{
public:
	/// <summary>
	/// シーンの切り替えを行う
	/// </summary>
	/// <param name="scene">切り替えたい次シーンのアドレス</param>
	void ChangeScene(Scene* scene);

	/// <summary>
	/// シーンを上に積む
	/// Updateで実行されるのは上に積まれたシーンのみ
	/// </summary>
	void PushScene(Scene* scene);
	
	/// <summary>
	/// 一番上のシーンを削除する
	/// ただし、その結果スタックが空になることのないように
	/// </summary>
	void PopScene();

	/// <summary>
	/// 各シーンの更新を行う
	/// </summary>
	/// <param name="input">入力ステート</param>
	void Update(Mouse& mouse);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 明示的に削除する
	/// </summary>
	void End();

	/// <summary>
	/// ゲームを終了させる
	/// </summary>
	void SetIsEnd() { m_isEnd = true; }
	/// <summary>
	/// ゲームを終了させるかどうか
	/// </summary>
	/// <returns>true:終了する false:終了しない</returns>
	bool GetIsEnd() const { return m_isEnd; }

	/// <summary>
	/// ウィンドウモードを取得
	/// </summary>
	/// <returns>　true:ウインドモード false:フルスクリーンモード</returns>
	bool GetIsWindouMode()const { return m_isWindouwMode; }
	/// <summary>
	/// ウィンドウモードを変更する
	/// </summary>
	/// <param name="isWindou">　true:ウインドモード false:フルスクリーンモード</param>
	void SetIsWindouMode(bool isWindou) { m_isWindouwMode = isWindou; }
private:
	//今実行中のシーンを切り替えていきたいので参照ではなくポインタとして宣言
	std::deque<Scene*> m_scenes;
	//ゲームを終了させる
	bool m_isEnd = false;
	//ゲーム画面モード
	bool m_isWindouwMode;
};

