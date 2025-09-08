#pragma once
#include "Vec2.h"
#include "Player.h"
#include "Rect.h"

class Shuriken
{
public:
		Shuriken();
	~Shuriken();

	void Init(int graphHandle);
	void Update();
	void Draw();

	bool IsActive() const;
	bool CheckCollision(const Rect& playerRect) const;
	Rect GetRect() const;
	// プレイヤーのポインタをセット
	void SetPlayer(Player* player);


private:
	//ハンドル関係
	int m_handle;		// 手裏剣のグラフィックハンドル
	int m_fireSeHandle;	//手裏剣発射SE

	//音量
	int m_fireSeVolume;	//手裏剣発射SEの音量

	//フラグ関係
	bool m_isActive;	// 手裏剣がアクティブかどうか1
	bool m_isFired;	// 手裏剣が発射されたかどうか

	//アニメーション関係
	int m_animFrame;	// 手裏剣のアニメーションフレーム

	Vec2 m_pos;			// 手裏剣の位置
	Vec2 m_velocity;	// 手裏剣の移動速度

	Rect m_colRect;		// 手裏剣当たり判定用の矩形

	//プレイヤー
	Player* m_pPlayer;	// プレイヤーのポインタ
};

