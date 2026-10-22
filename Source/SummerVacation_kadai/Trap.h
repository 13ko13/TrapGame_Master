#pragma once

#include "Rect.h"
#include "Vec2.h"

class Player;

class Trap
{
public:
	Trap();
	~Trap();

	void Init(
		const Vec2& RtrapPos, const Vec2& UtrapPos,
		const Vec2& LtrapPos, const Vec2& BtrapPos,
		const Vec2& velocity, 
		int RgraphHandle, int UgraphHnadle, int LgraphHandle, int BtrapHandle);
	void End();
	void Update();
	void Draw();
	
	void SetPlayer(Player* pPlayer);

	bool IsRtrapActive() const;
	bool IsBtrapActive() const;
	bool IsFireBtrap() ;

	Rect GetRightRect() const;	// 右向きトラップの当たり判定矩形
	Rect GetUpRect() const;		// 上向きトラップの当たり判定矩形
	Rect GetLeftRect() const;	// 左向きトラップの当たり判定矩形
	Rect GetBottomRect() const;	// 下向きトラップの当たり判定矩形

private:
	//ハンドル関係
	int m_Rhandle;			// トラップの右向きグラフィックハンドル
	int m_Uhandle;			// トラップの上向きグラフィックハンドル
	int m_Lhandle;			// トラップの左向きグラフィックハンドル
	int m_BtrapHandle;		// トラップの下向きグラフィックハンドル

	//フラグ関係
	bool m_isRtrapActive;		// 右向きトラップがアクティブかどうか
	bool m_isBtrapActive;		//下向きトラップがアクティブかどうか
	bool m_isBtrapFire;			//下向きトラップを発射するかどうか

	Vec2 m_RtrapPos;        // 右向きトラップの位置
	Vec2 m_UtrapPos;        // 上向きトラップの位置
	Vec2 m_LtrapPos;        // 左向きトラップの位置
	Vec2 m_BtrapPos;        // 下向きトラップの位置
	Vec2 m_velocity;		// トラップの移動速度

	Rect m_RtrapColRect;	// 右向きトラップ当たり判定用の矩形
	Rect m_UtrapColRect;	// 上向きトラップ当たり判定用の矩形
	Rect m_LtrapColRect;	// 左向きトラップ当たり判定用の矩形
	Rect m_BtrapColRect;	// 下向きトラップ当たり判定用の矩形

	Player* m_pPlayer;	//Playerのポインタ
};

