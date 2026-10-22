#pragma once

#include "Vec2.h"
#include "Rect.h"

class FallPlatTrap
{
public:
		FallPlatTrap();
	~FallPlatTrap();
	void Init(const Vec2& pos,int delayFrames, int graphHandle);
	void Update(const Rect& playerRect);
	void Draw();
	bool IsActive() const;
	Rect GetRect() const;

private:
	void DrawFallPlat(int DrawPosX, int DrawPosY, int posX, int posY);

private:
	Vec2 m_pos;        // トラップの位置
	int m_delayFrames; // トラップが落下するまでのフレーム数
	int m_currentFrames; // 現在のフレーム数
	int m_handle;      // トラップのグラフィックハンドル
	bool m_isActive;   // トラップがアクティブかどうか
	bool m_playerOnPlatform; // プレイヤーがプラットフォーム上にいるかどうか
	bool m_isVisible;  // トラップが表示されているかどうか
	Rect m_colRect;    // 当たり判定用の矩形
};

