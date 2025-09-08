#pragma once
#include "Vec2.h"
#include "Player.h"

class Shot
{
public:
	Shot();
	~Shot();

	void Init();
	void Update();
	void Draw();

	void SetInfo(const Vec2& pos, bool isTurn);

	Vec2 GetPos() const { return m_pos; };
	Rect GetColRect() const { return m_colRect; }
private:
	int m_handle;	//グラフィックハンドル

	Vec2 m_pos;		//弾の位置
	Vec2 m_move;	//移動量
	Rect m_colRect;	//当たり判定用の矩形
};

