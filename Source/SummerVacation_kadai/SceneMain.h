#pragma once
#include "Bg.h"
#include "Player.h"
#include "Shot.h"
#include "TrapManager.h"
#include "PlatformManager.h"
#include "MoveSpikeMgr.h"
#include "Shuriken.h"
#include "Goal.h"
#include "SceneTitle.h"
#include "ParticleMgr.h"

class Player;
class Shot;
class Bg;
class Rock;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	void UpdateShot(); //弾更新
	void DeleteShot(int index); //弾削除

	//シーケンスごとにUpdate処理を切り替える
	void UpdateTitle();		//タイトル
	void UpdateFadeIn();	//フェードイン
	void UpdateGame();		//ゲームプレイ

	void UpdateClear();		//クリア
	void UpdateGameOver();	//ゲームオーバー

private:
	//シーケンス
	enum Seq
	{
		SeqTitle,	//タイトル
		SeqFadeIn,	//フェードイン
		SeqGame,	//ゲームプレイ
		SeqClear,	//クリア
		SeqGameOver,//ゲームオーバー

		SeqNum	//シーケンスの数
	};

public:
	//スタートを押したかどうかのフラグ
	bool m_isStartPressed;

private:
	//フレーム
	int m_frame;	//フレームを計測
	float m_clearFrame; //クリアしてからのフレーム

	//使用するグラフィック
	int m_playerIdleGraphHandle;	//プレイヤーのアイドルグラフィック
	int m_playerWalkGraphHandle;	//プレイヤーの歩きグラフィック
	int m_playerShotGraphHnadle;	//プレイヤーの弾撃ちグラフィック
	int m_playerJumpGraphHandle;	//プレイヤーのジャンプグラフィック
	int m_playerDJumpGraphHandle;	//プレイヤーの二段ジャンプグラフィック
	int m_tileGraphHandle;			//マップチップのグラフィック
	int m_bgGraphHandle;			//背景のグラフィック
	int m_bulletGraphHandle;		//弾のグラフィック
	int m_RtrapGraphHandle;			//右向きトラップのグラフィック
	int m_UtrapGraphHandle;			//上向きトラップのグラフィック
	int m_LtrapGraphHandle;			//左向きトラップのグラフィック
	int m_BtrapGraphHandle;			//下向きトラップのグラフィック
	int m_platformGraphHandle;		//プラットフォームのグラフィック
	int m_moveSpikeGraphHandle;		//移動トゲのグラフィック
	int m_shurikenGraphHandle;		//手裏剣のグラフィック
	int m_goalGraphHandle;			//ゴールのグラフィック
	int m_rockGraphHandle;			//岩のグラフィック

	//フォントハンドル
	int m_clearFontHandle;		//クリア表示用フォントハンドル
	int m_backTitleFontHandle;	//タイトル背景用フォントハンドル
	int m_gameoverFontHandle; //ゲームオーバー用フォントハンドル

	//サウンドハンドル
	int m_mainBgmHandle;		//メインBGMハンドル
	int m_clearSeHandle;		//クリアSEハンドル
	int m_fireSpikeSeHandle;	//トゲ発射時SE
	int m_deadSeHandle;			//死亡時SE

	//サウンドの音量
	int m_mainBgmVolume;			//メインBGMの音量
	int m_clearSeVolume;			//クリアSE音量
	int m_fireSpikeSeVolume;		//トゲ発射時SE音量
	int m_deadSeVolume;				//死亡時SE音量

	//フラグ関係

	//トゲ発射済みフラグ
	bool m_isRtrapFired;			// 一度だけ生成するフラグ(右向きトゲ)
	//プラットフォームフラグ
	bool m_isPlatformSpawned;		//一度だけ生成するフラグ
	//トゲ設置済みフラグ
	bool m_isUtrapSpawned;			//一度だけ生成するフラグ(下向きトゲ)
	bool m_isLtrapSpawned;			//一度だけ生成するフラグ(左向きトゲ)
	bool m_isBtrapSpawned;			//一度だけ生成するフラグ(下向きトゲ)
	//移動トゲフラグ
	bool m_isMoveSpikeSpawned;		//一度だけ生成するフラグ
	//プレイヤー死亡フラグ
	bool m_isDead;					//死んでいるか死んでいないか
	//死亡処理を行ったかどうか
	bool m_isDeadActive;
	//岩発動済みフラグ
	bool m_isRockFired;				//一度だけ生成するフラグ

	//現在のシーケンス
	Seq m_gameSeq;
	int m_frameCount;

	//フェードのフレーム数 0:真っ暗
	int m_fadeFrame;

	//プレイヤー
	Player* m_pPlayer;
	//背景
	Bg* m_pBg;
	//弾
	Shot* m_pShot[5];
	//手裏剣
	Shuriken* m_pShuriken;
	//トラップマネージャー
	TrapManager m_trapManager;
	//プラットフォームマネージャー
	PlatformManager m_platformManager;
	//移動トゲマネージャー
	MoveSpikeMgr m_moveSpikeMgr;
	//ゴール
	Goal m_goal;
	//タイトルシーン
	SceneTitle m_sceneTitle;
	//パーティクルマネージャー
	ParticleMgr m_particleMgr;
	//岩
	Rock* m_pRock;
};