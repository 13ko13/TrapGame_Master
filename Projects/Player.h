#pragma once
#include "Vec2.h"
#include "Character.h"
#include "Shot.h"
#include "Bg.h"
#include "PlatformManager.h"
#include "Trap.h"

class Shot;
class TrapManager;

class Player : public Character
{
public:
	Player();
	virtual ~Player();

	virtual void Init(int handle,int handleRun, int handleWalk,int handleShot,
					  int handleJump, int handleDJump) ;
	void End();
	virtual void Update() override;
	virtual void Draw()override;
	Shot* CreateShot();

private:
	//移動
	void Move();
	//ジャンプ
	void Jump();
	//弾のクールタイム
	void ShotCT(); 

private:
	//ハンドル
	int m_jumpSeHandle;		//ジャンプ音
	int m_shotSeHandle;		//弾発射音

	//音量
	int m_jumpSeVolume;		//ジャンプ音量
	int m_shotSeVolume;		//弾発射音量

	//押された瞬間だけをとるための前回の入力状態
	int m_prevInput = 0;
	//現在のジャンプした回数(二段ジャンプ用)
	int m_jumpNum;

	//プレイヤーが入力状態かどうかのフラグ
	bool m_isInput;
	//プレイヤーが弾を発射している状態かどうかのフラグ
	bool m_isShotInput;
	//プレイヤーが二段ジャンプをしているかのフラグ
	bool m_isDoubleJump;

	//アニメーション関連
	int m_animFrame;	//アニメーションのフレーム数
	int m_shotAnimTime; //Shot状態を維持するための時間カウント

	//時間関連
	int m_time;	//時間を計測

	//トラップ
	Trap m_trap;
	//トラップマネージャー
	TrapManager* m_pTrapManager;
};