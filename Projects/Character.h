#pragma once
#include "Rect.h"
#include "Vec2.h"

class Rect;
class Bg;
class PlatformManager;

class Character
{
public:
	Character();
	virtual ~Character();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	Vec2 GetPos() const { return m_pos; }
	Rect GetColRect() const { return m_colRect; }

	//PlatformManagerを外部からセットできるようにする
	void SetPlatformManager(PlatformManager* pPlatformManager) 
	{ 
		m_pPlatformMgr = pPlatformManager; 
	}

public:
	//プレイヤーの現在の行動
	enum class PlayerState
	{
		Idle,
		Walk,
		Shot,
		Jump,
		DoubleJump,
		Fall
	};

	PlayerState m_state;
	Bg* m_pBg;

protected:	
	void Gravity();
	//マップチップとの当たり判定処理
	void CheckHitMap(Rect& chipRect);

protected:
	int m_handle;
	//プレイヤーの入力待機中のグラフィックハンドル
	int m_handleIdle;
	//プレイヤーの歩き入力中のグラフィックハンドル
	int m_handleWalk;
	//プレイヤーの弾撃ち入力中のグラフィックハンドル
	int m_handleShot;
	//プレイヤーのジャンプ中のグラフィックハンドル
	int m_handleJump;
	//プレイヤーの二段ジャンプ中のグラフィックハンドル
	int m_handleDJump;

	//左右反転するか
	bool m_isTurn;
	//地面に着地しているか
	bool m_isGround;

	Vec2 m_pos;		//座標
	Vec2 m_move;	//移動
	Rect m_colRect;	//当たり判定用の矩形

	PlatformManager* m_pPlatformMgr; // プラットフォームの状態管理
};