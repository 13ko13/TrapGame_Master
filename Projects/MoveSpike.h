#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "DxLib.h"

class MoveSpike
{
public:
	MoveSpike();
	~MoveSpike();

	void Init(const Vec2& pos, const Vec2& velocity, int graphHandle);
	void Update();
	void Draw();
	void End();

	void IsReturn(); // トゲが戻るタイミングかどうかを判定する
	bool IsActive() const;
	Rect GetRect() const;

private:
	//ハンドル関係
	int m_handle;		// トゲのグラフィックハンドル

	//フラグ関係
	bool m_isActive;	// トゲがアクティブかどうか
	//移動トゲが上下に動くタイミングのフラグ
	bool m_isReturn; // トゲが戻るタイミングかどうか

	Vec2 m_pos;			// トゲの位置
	Vec2 m_velocity;	// トゲの移動速度

	Rect m_colRect;		// トゲ当たり判定用の矩形
};

