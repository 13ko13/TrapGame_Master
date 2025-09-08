#pragma once

class SceneMain;

class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();

	void Init();
	void End();
	void Update();
	void Draw();

	void SetSceneMain(SceneMain* pSceneMain);

	void UpdateTitle();		//タイトル
	void UpdateFadeIn();	//フェードイン

	//シーケンス
	enum Seq
	{
		SeqTitle,	//タイトル
		SeqFadeIn,	//フェードイン

		SeqNum	//シーケンスの数
	};

private:

	//使用するグラフィック
	int m_titleGraphHandle; //タイトルのグラフィックハンドル
	int m_bgGraphHandle;    //背景のグラフィックハンドル
	
	//フォントハンドル
	int m_startFontHandle; //PressStartのグラフィックハンドル

	//サウンドハンドル
	int m_titleBgmHandle;
	int m_startGameSeHandle;		//ゲームをスタートした時の音

	//サウンドの音量
	int m_titleBgmVolume;	//BGMの音量
	int m_startSeVolume;	//ゲームスタートの音量
	
	//シーケンス管理
	int m_currentSeq;	//現在のシーケンス

	//フェード関係
	int m_fadeAlpha;	//フェード用のアルファ値

	//時間
	int m_time;			//時間を計測

	//フラグ関係
	bool m_isFadeIn;	//フェードイン中かどうかのフラグ
	bool m_isPressStart;//スタートを押したかどうか

	//アニメーション関係
	int m_pressFrame;   //PressStartの点滅用フレームカウント

	//シーンメイン
	SceneMain* m_pSceneMain;
};

