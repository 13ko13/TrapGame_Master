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
	void DrawBg(); //�w�i�\��

private:
	//�摜�Ɋ܂܂��}�b�v�`�b�v�̐�
	int m_graphChipNumX;
	int m_graphChipNumY;
	int m_mapHandle;	//�}�b�v�`�b�v�̉摜
	int m_bgHandle; //�w�i�̉摜

	Vec2 m_pos;	//�w�i�̈ʒu
};

