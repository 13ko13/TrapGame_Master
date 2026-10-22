#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "DxLib.h"
#include "Player.h"

class Goal
{
public:
	Goal();
	~Goal();

	void Init(int graphHandle);
	void Update();
	void Draw();

	bool IsClear() const;
	Rect GetColRect() const;
	void SetPlayer(Player* player);

private:
	//ハンドル関係
	int m_handle;		// ゴールのグラフィックハンドル

	//アニメーション関係
	int m_animFrame;	// ゴールのアニメーションフレーム

	//位置関係
	Vec2 m_pos;			// ゴールの位置

	//当たり判定関係
	Rect m_colRect;		// ゴール当たり判定用の矩形

	//プレイヤー
	Player* m_pPlayer;	// プレイヤーのポインタ

public:
	//フラグ関係
	bool m_isClear;
};