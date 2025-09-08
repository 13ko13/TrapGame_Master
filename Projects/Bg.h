#pragma once
#include "Rect.h"
#include "Vec2.h"

class Bg
{
public:
	Bg();
	~Bg();

	void Init();
	void End();
	void Update();
	void Draw();
	void DrawMapChip();
	bool IsCollision(Rect rect, Rect& chipRect);

private:
	void DrawBg(); //背景表示

private:
	//画像に含まれるマップチップの数
	int m_graphChipNumX;
	int m_graphChipNumY;
	int m_mapHandle;	//マップチップの画像
	int m_bgHandle; //背景の画像

	Vec2 m_pos;	//背景の位置
};

