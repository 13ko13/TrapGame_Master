#pragma once
#include "Vec2.h"
#include "Player.h"
#include "Rect.h"

class Rock
{
public:
	Rock();
	~Rock();

	void Init(int graph);
	void End();
	void Update();
	void Draw();
	
	void SetPlayer(Player* player);
	bool IsActive() const;
	bool CheckCollision(const Rect& playerRect) const;
	Rect GetRect() const;


public:
	//ハンドル関係
	int m_graphHandle;

	//アニメーション関係
	int m_animFrame;

	//フラグ関係
	bool m_isActive;

	//インスタンス
	Vec2 m_pos;
	Vec2 m_vel;
	Rect m_colRect;
	Player* m_pPlayer;
};

