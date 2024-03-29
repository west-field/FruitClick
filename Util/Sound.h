#pragma once
#include <unordered_map>

//ゲーム中使用する音
enum class SoundId
{
	Cursor,			//カーソル移動音
	Determinant,	//決定ボタン
	Back,			//戻るボタン
	
	Count,			//カウントダウン音
	Start,			//スタート音
	Point,			//得点取得音
	
	PlayerMove,		//プレイヤー移動音
	Hit,			//接触音
	
	FruitCreate,	//フルーツ生成音
	FruitClick,		//フルーツクリック音
	FruitDelete,	//フルーツ破壊音

	SoundId_Max
};

//サウンドマネージャー
class SoundManager
{
private:
	SoundManager();
	
	//コピーも代入も禁止する
	SoundManager(const SoundManager&) = delete;
	void operator= (const SoundManager&) = delete;

	/// <summary>
	/// サウンドハンドルを取得
	/// </summary>
	/// <param name="id">サウンド名</param>
	/// <param name="fileName">サウンドファイル名</param>
	void LoadSoundFile(SoundId id,const wchar_t* fileName);
	
	/// <summary>
	/// サウンド情報のロード
	/// </summary>
	void LoadSoundConfig();

	std::unordered_map<SoundId, int> m_nameAndHandleTable;//サウンドハンドル

	//変更したサウンド情報をファイルに書き込むための構造体
	struct SoundConfigInfo
	{
		unsigned short volumeSE;//0〜255
		unsigned short volumeBGM;//0〜255
	};

	int m_volumeSE = 255;//初期SE音量
	int m_volumeBGM = 255;//初期BGM音量

public:
	~SoundManager();

	/// <summary>
	/// SoundManager使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns>実体の参照を返す</returns>
	static SoundManager& GetInstance()
	{
		static SoundManager instance;//唯一の実体
		return instance;//それの参照を返す
	}

	/// <summary>
	/// サウンド情報のセーブ
	/// </summary>
	void SaveSoundConfig();

	/// <summary>
	/// 指定のサウンドを鳴らす
	/// </summary>
	/// <param name="name">サウンド名</param>
	void Play(SoundId id, int volume = 200);

	/// <summary>
	/// BGMを鳴らす
	/// </summary>
	/// <param name="soundH">サウンドハンドル</param>
	void PlayBGM(int soundH) const;

	/// <summary>
	/// SEの音量を設定する
	/// </summary>
	/// <param name="volume">音量</param>
	void SetSEVolume(int volume);

	//SEの音量を取得する
	int GetSEVolume()const;

	/// <summary>
	/// BGMの音量を設定する
	/// </summary>
	/// <param name="volume">音量</param>
	/// <param name="soundH">サウンドハンドル</param>
	void SetBGMVolume(int volume,int soundH);

	//BGMの音量を取得する
	int GetBGMVolume()const;

	/// <summary>
	/// サウンドを止める
	/// </summary>
	/// <param name="id">サウンド名</param>
	void StopBgm(SoundId id);
};


